#include <string.h>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "mbedtls/md5.h"
#include "sid_player.h"
#include <lwip/sockets.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_task_wdt.h"

#define ZST_PLAYER_NO_SONG       0
#define ZST_PLAYER_CHECK_CMD     1
#define ZST_PLAYER_LOAD_TUNE     2
#define ZST_PLAYER_PLAY_TUNE     3

static const char *TAG = "SIDBOY";

#define LOW_BYTE(x)     ((unsigned char)((x)&0xFF))
#define HIGH_BYTE(x)    ((unsigned char)(((x)>>8)&0xFF))

volatile uint32_t player_state = ZST_PLAYER_NO_SONG;

//static void periodic_timer_callback(void* arg);

/* WiFi station Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/* The examples use WiFi configuration that you can set via project configuration menu

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_ESP_WIFI_SSID      "Vodafone-F36C"
#define EXAMPLE_ESP_WIFI_PASS      "NF83cJG3qH8PNUEQ"
#define EXAMPLE_ESP_MAXIMUM_RETRY  3

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1


static int s_retry_num = 0;
static int flag_wifi_connected = 0;
int player_socket;

struct sid_player player;
//struct sid_player *player;

TaskHandle_t  Core0TaskHandle;  
TaskHandle_t  Core1TaskHandle; 

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        flag_wifi_connected = 1;
    }
}

void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
	     .threshold.authmode = WIFI_AUTH_WPA2_PSK,

            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

    /* The event will not be processed after unregister */
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
    vEventGroupDelete(s_wifi_event_group);
}

#if 0
void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();
}
#endif

#define LEDC_0_PIN 23 // CLOCK 6581

uint8_t *sid_memory;

/* Core 0: WIFI;Menü;Ladelogik; Dateisystem Core */
void CoreTask0(void * parameter) 
{ 
    esp_err_t ret;
    esp_vfs_spiffs_conf_t conf;
    size_t total = 0, used = 0;
    u_long bytes_available;
	uint8_t cmd_buffer[5];

#if 0
    ESP_LOGI(TAG, "Initializing SPIFFS");

    //Initialize NVS
    ret = nvs_flash_init();
    
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    conf.base_path = "/spiffs";
    conf.partition_label = NULL;
    conf.max_files = 5;
    conf.format_if_mount_failed = false;

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    ret = esp_vfs_spiffs_register(&conf);

    if(ret != ESP_OK) 
    {
        if(ret == ESP_FAIL) 
        {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) 
        {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else 
        {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }

        return;
    }

    ret = esp_spiffs_info(NULL, &total, &used);

    if(ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } 
    else 
    {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }
#endif
    wifi_init_sta();

    while(!flag_wifi_connected)
        usleep(1000 * 20);
    
    printf("Bin verbunden!\n");

    int addr_family = AF_INET;
    int ip_protocol = 0;
    struct sockaddr_in dest_addr;
    struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
    dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr_ip4->sin_family = AF_INET;
    dest_addr_ip4->sin_port = htons(6581);
    ip_protocol = IPPROTO_IP;
    //Create a socket
    int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (listen_sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            vTaskDelete(NULL);
            return;
    }
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    ESP_LOGI(TAG, "Socket created");
    //Bind the socket to an address
    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
            ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
            //   goto CLEAN_UP;
    }
    ESP_LOGI(TAG, "Socket bound, port %d", 6581);
    //Listen for connections
    err = listen(listen_sock, 1);
    if (err != 0) {
            ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
            // goto CLEAN_UP;
    }
    ESP_LOGI(TAG, "Socket listening");
    struct sockaddr_in source_addr;
    uint addr_len = sizeof(source_addr);
    //Accept a connection, This call typically blocks until a client connects with the server.
    player_socket = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
    if (player_socket < 0) {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            return;
    }

    sid_memory = (uint8_t*) malloc(65536);
 
    init_sid_player(&player);

    player_state = ZST_PLAYER_NO_SONG;
   
    int t_sekunden = 0;

    do
    {
        switch(player_state)
        {
            case ZST_PLAYER_NO_SONG:
            {	
                ioctl(player_socket, FIONREAD, &bytes_available);
            
                if(bytes_available >= 5)
                {
                    printf("ZST_PLAYER_CHECK_CMD\n");
                    player_state = ZST_PLAYER_CHECK_CMD;
                    break;
                }
            
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                break;
            }

            case ZST_PLAYER_CHECK_CMD:
            {
                recv(player_socket, (char*) &cmd_buffer[0], 5, 0);

                if(cmd_buffer[0] == 'L')
                {
                    player_state = ZST_PLAYER_LOAD_TUNE;
                    break;
                }

                break;
            }

            case ZST_PLAYER_LOAD_TUNE:
            {
                union dword filesize;

                filesize.s.b1 = cmd_buffer[1];
                filesize.s.b2 = cmd_buffer[2];
                filesize.s.b3 = cmd_buffer[3];
                filesize.s.b4 = cmd_buffer[4];
                
                uint32_t idx = 0;
                uint32_t read_len;
                uint32_t bytes_to_read = filesize.value;

                do
                {
                    read_len = recv(player_socket, (char*)&sid_memory[idx], filesize.value, 0);
                    idx += read_len;
                    bytes_to_read -= read_len;
                } while(bytes_to_read != 0);
            
                printf("LADE LIED!\n");
                
                while(player.flag_emulate_frame)
                      printf("WARTE\n");
                
                load_sid_from_memory(&player, filesize.value, sid_memory);
                init_sid_tune(&player, 0);

                player_state = ZST_PLAYER_PLAY_TUNE;
                t_sekunden = 0;
                player.system_6502.cpu.reg.anz_PlayJSR = 0;

                break;
            }

            case ZST_PLAYER_PLAY_TUNE:
            {
                ioctl(player_socket, FIONREAD, &bytes_available);

                if (bytes_available != 0)
                {
                    player_state = ZST_PLAYER_CHECK_CMD;
                    break;
                }
                
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                t_sekunden++;

#if 1

#if 0
                if((player.system_6502.bus.mem[0xdd0d] & (1 << 0)))
                    printf("DD0D: Interrupt A an\n");
                else printf("DD0D: Interrupt A aus\n");
               
                if((player.system_6502.bus.mem[0xdd0e] & (1 << 0)))
                    printf("DD0E: Stop Timer A\n");
                else printf("DD0E: Start Timer A\n");


                 if((player.system_6502.bus.mem[0xdd0e] & (1 << 3)))
                    printf("DD0E: Timer A One Shoot\n");    
                else printf("DD0E: Timer A Continues\n"); 

                if((player.system_6502.bus.mem[0xdd0e] & (1 << 4)))
                    printf("DD0E: Timer A Load start value into timer.\n");

                if((player.system_6502.bus.mem[0xdd0e] & (1 << 5)))
                    printf("DD0E: Timer A Timer counts positive edges on CNT pin.\n");
                else printf("DD0E: Timer A Timer Timer counts system cycles.\n");
#endif

                printf("timeA=%d\n", player.system_6502.cia2.timerA_latch);


                    printf("DD0F: %d\n", player.system_6502.bus.mem[0xDD0F]);

                  printf("DD0E: %d\n", player.system_6502.bus.mem[0xDD0E]);

                printf("FPS = %d\n", player.system_6502.cpu.reg.anz_PlayJSR / t_sekunden);
            
            
            //	player->system_6502.bus.mem[0xD011] = HIGH_BYTE(scanline) & 0x80;
            //	player->system_6502.bus.mem[0xD012] = LOW_BYTE(scanline);
              printf("0xD011: %d\n", player.system_6502.bus.mem[0xD011]);
                printf("0xD012: %d\n", player.system_6502.bus.mem[0xD012]);

                  printf("0xd01a: %d\n", player.system_6502.bus.mem[0xd01a]);

                    

                  printf("0xdd0d: %d\n", player.system_6502.bus.mem[0xdd0d]);


              #endif

                #if 0
                printf("-------SET-RASTER-LINE----------\n");
                printf("D011: %d\n", player.system_6502.bus.mem[0xD011]);
                printf("D012: %d\n", player.system_6502.bus.mem[0xD012]);
                printf("---------RASTER-LINE--IRQ--------\n");
              
                printf("0314: %d\n", player.system_6502.bus.mem[0x0314]);
                printf("0315: %d\n", player.system_6502.bus.mem[0x0315]);

                printf("---------NMI--IRQ--------\n");
              
                printf("0318: %d\n", player.system_6502.bus.mem[0x0318]);
                printf("0319: %d\n", player.system_6502.bus.mem[0x0319]);


                printf("-------CIA1-----------------\n");
                printf("DC04: %d\n", player.system_6502.bus.mem[0xDC04]);
                printf("DC05: %d\n", player.system_6502.bus.mem[0xDC05]);
                printf("DC06: %d\n", player.system_6502.bus.mem[0xDC06]);
                printf("DC07: %d\n", player.system_6502.bus.mem[0xDC07]);
                printf("DC0D: %d\n", player.system_6502.bus.mem[0xDC0D]);
                printf("DC0E: %d\n", player.system_6502.bus.mem[0xDC0E]);
                printf("DC0F: %d\n", player.system_6502.bus.mem[0xDC0F]);
              //  #endif
               
                printf("-------CIA2-----------------\n");
                
                printf("DD04: %d\n", player.system_6502.bus.mem[0xDD04]);
                printf("DD05: %d\n", player.system_6502.bus.mem[0xDD05]);
                printf("DD06: %d\n", player.system_6502.bus.mem[0xDD06]);
                printf("DD07: %d\n", player.system_6502.bus.mem[0xDD07]);
                printf("DD0D: %d\n", player.system_6502.bus.mem[0xDD0D]);
                printf("DD0E: %d\n", player.system_6502.bus.mem[0xDD0E]);
                printf("DD0F: %d\n", player.system_6502.bus.mem[0xDD0F]);

                printf("-------VIC-----------------\n");
                  printf("D01a: %d\n", player.system_6502.bus.mem[0xD01a]);
            

                   printf("DD0E: %d\n", player.system_6502.bus.mem[0xDD0e]);
            

              //  printf("%d, %d\n", player.system_6502.bus.mem[player.system_6502.cpu.reg.pc.value],player.system_6502.cpu.reg.pc.value);
                
                  printf("%d\n", (int)player.system_6502.cpu.reg.max_cyles);
              
                 //printf("%d\n", player.system_6502.cpu.reg.anz_PlayJSR / t_sekunden);
              
                //printf("DC04: %d\n", player.system_6502.bus.mem[0xdc04]);
                //printf("DC0D: %d\n", player.system_6502.bus.mem[0xdc05]);
                #endif
                
                break;
            }
        }
   } while(1);

    // All done, unmount partition and disable SPIFFS
    esp_vfs_spiffs_unregister(NULL);
    ESP_LOGI(TAG, "SPIFFS unmounted");
}

static portMUX_TYPE my_mutex;
  

/* Core 1: SID-Emulator */
IRAM_ATTR void CoreTask1(void * parameter) 
{ 
    esp_err_t result;
    ledc_timer_config_t ledc_timer;
    ledc_channel_config_t ledc_channel;
   
    #if 0
    uint32_t ccount;
    RSR(CCOUNT, ccount);
    #endif

    /* GPIO konfigurieren */
    gpio_pad_select_gpio(22);  // D0
    gpio_pad_select_gpio(23);  // CLK
    gpio_pad_select_gpio(21);  // D7
    gpio_pad_select_gpio(19);  // CS
    gpio_pad_select_gpio(18);  // A0
    gpio_pad_select_gpio(5);   // A1
    gpio_pad_select_gpio(15);  // A2
    gpio_pad_select_gpio(13);  // A3
    gpio_pad_select_gpio(4);   // A4
    gpio_pad_select_gpio(14);  // D1
    gpio_pad_select_gpio(27);  // D2
    gpio_pad_select_gpio(26);  // D3
    gpio_pad_select_gpio(25);  // D4
    gpio_pad_select_gpio(33);  // D5
    gpio_pad_select_gpio(32);  // D6
 
    gpio_set_direction(22, GPIO_MODE_OUTPUT); 
    gpio_set_direction(23, GPIO_MODE_OUTPUT); 
    gpio_set_direction(21, GPIO_MODE_OUTPUT); 
    gpio_set_direction(19, GPIO_MODE_OUTPUT); 
    gpio_set_direction(18, GPIO_MODE_OUTPUT); 
    gpio_set_direction(5, GPIO_MODE_OUTPUT); 
    gpio_set_direction(15, GPIO_MODE_OUTPUT); 
    gpio_set_direction(13, GPIO_MODE_OUTPUT); 
    gpio_set_direction(4, GPIO_MODE_OUTPUT); 
    gpio_set_direction(12, GPIO_MODE_OUTPUT); 
    gpio_set_direction(14, GPIO_MODE_OUTPUT); 
    gpio_set_direction(27, GPIO_MODE_OUTPUT); 
    gpio_set_direction(26, GPIO_MODE_OUTPUT); 
    gpio_set_direction(25, GPIO_MODE_OUTPUT); 
    gpio_set_direction(33, GPIO_MODE_OUTPUT); 
    gpio_set_direction(32, GPIO_MODE_OUTPUT); 
   
    /* 1MHrz Clock erzeugen */
    ledc_timer.clk_cfg = LEDC_USE_APB_CLK;
    ledc_timer.speed_mode = LEDC_HIGH_SPEED_MODE;       // timer mode
    ledc_timer.duty_resolution = LEDC_TIMER_2_BIT;      // resolution of PWM duty
    ledc_timer.timer_num = LEDC_TIMER_0;                // timer index
  //  ledc_timer.freq_hz = 1000000;                       // frequency of PWM signal
    ledc_timer.freq_hz = 985000;                       // frequency of PWM signal PAL
    
    // Set configuration of timer0 for high speed channels
    result = ledc_timer_config(&ledc_timer);
  
    if(result == ESP_OK)
        printf("frequency: %d", ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0));
  
    ledc_channel.gpio_num = LEDC_0_PIN;
    ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_channel.channel = LEDC_CHANNEL_0;
    ledc_channel.intr_type = LEDC_INTR_DISABLE;
    ledc_channel.timer_sel = LEDC_TIMER_0;
    ledc_channel.duty = 2;
    ledc_channel.hpoint = 0;

    // Set LED Controller with previously prepared configuration
    ledc_channel_config(&ledc_channel);
   
    gpio_set_level(19, 1); // CS

    vTaskSuspendAll();

    portDISABLE_INTERRUPTS();

 	uint32_t count_1;
    uint32_t flag_firstTime = 0;

	asm volatile ( "rsr %0, ccount" : "=a" (count_1));
	player.system_6502.cpu.reg.start_time = count_1;
	player.system_6502.cpu.reg.clocks = 0;

    for(;;) 
    { 
        if(player_state != ZST_PLAYER_PLAY_TUNE)
            continue;

        if(!flag_firstTime)
        {
            flag_firstTime = 1;
            asm volatile ( "rsr %0, ccount" : "=a" (count_1));
            player.system_6502.cpu.reg.start_time = count_1;
        }

        play_sid_tune(&player);
    }
}

void app_main(void)
{ 
    xTaskCreatePinnedToCore(CoreTask0,"CPU_0",5000,NULL,1,&Core0TaskHandle,0);
    xTaskCreatePinnedToCore(CoreTask1,"CPU_1",5000,NULL,1,&Core1TaskHandle,1);
    //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout ...
}

#if 0
static void periodic_timer_callback(void* arg)
{
    int start;
    int end;

   portMUX_TYPE myMutex = portMUX_INITIALIZER_UNLOCKED;
   taskENTER_CRITICAL(&myMutex);
    
   start = xthal_get_ccount();
   uint32_t scanline;
    //int64_t time_since_boot = esp_timer_get_time();
   
     // ESP_LOGI(TAG, "Periodic timer called_1, time since boot: %lld us", time_since_boot);

    if(player_state != ZST_PLAYER_PLAY_TUNE)
       {
         taskEXIT_CRITICAL(&myMutex);
        return;
        }

     //ESP_LOGI(TAG, "Periodic timer called_2, time since boot: %lld us", time_since_boot);

    scanline = player.system_6502.cpu.reg.clocks / 63;
	player.system_6502.bus.mem[0xd011] = HIGH_BYTE(scanline) & 0x80;
	player.system_6502.bus.mem[0xd012] = LOW_BYTE(scanline);
	tick_6502_system(&player.system_6502);

    if(player.system_6502.cpu.reg.clocks >= 19656)
        player.system_6502.cpu.reg.clocks = 0;

      end = xthal_get_ccount();
  
      ESP_LOGI(TAG, "Periodic timer called_2, time since boot: %d us", (end-start));
  taskEXIT_CRITICAL(&myMutex);
 

    
     #if 0

uint32_t scanline = 0;

	uint32_t scanline_irq_at = 0;
	uint8_t flag_irq_generated = 0;
	do
	{
		scanline = player->system_6502.cpu.reg.clocks / 63;
		player->system_6502.bus.mem[0xD011] = HIGH_BYTE(scanline) & 0x80;
		player->system_6502.bus.mem[0xD012] = LOW_BYTE(scanline);
		tick_6502_system(&player->system_6502);

		if ((player->system_6502.bus.mem[0xd01a] & (1 << 0)) != 0)
		{
			if (!player->system_6502.cpu.reg.flags.interrupt)
			{
				if (!flag_irq_generated)
				{
					if (scanline == scanline_irq_at)
					{
						generate_irq_6502_system(&player->system_6502);
						flag_irq_generated = 1;
					}
				}
				else
				{
					if (scanline != scanline_irq_at)
					{
						flag_irq_generated = 0;
					}
				}
			}
		}
	} while (player->system_6502.cpu.reg.clocks < max_cyles);
     #endif


     #if 0

    portMUX_TYPE myMutex = portMUX_INITIALIZER_UNLOCKED;
	taskENTER_CRITICAL(&myMutex);
      int start = xthal_get_ccount();
      __asm__ __volatile__("nop;"); // Bug workaround (I found this snippet somewhere in this forum)
      __asm__ __volatile__("nop;"); // Bug workaround (I found this snippet somewhere in this forum)
      __asm__ __volatile__("nop;"); // Bug workaround (I found this snippet somewhere in this forum)
      __asm__ __volatile__("nop;"); // Bug workaround (I found this snippet somewhere in this forum)
      __asm__ __volatile__("nop;"); // Bug workaround (I found this snippet somewhere in this forum)
      __asm__ __volatile__("nop;"); // Bug workaround (I found this snippet somewhere in this forum)
      __asm__ __volatile__("nop;"); // Bug workaround (I found this snippet somewhere in this forum)
      __asm__ __volatile__("nop;"); // Bug workaround (I found this snippet somewhere in this forum)
      __asm__ __volatile__("nop;"); // Bug workaround (I found this snippet somewhere in this forum)
      __asm__ __volatile__("nop;"); // Bug workaround (I found this snippet somewhere in this forum)
        int end = xthal_get_ccount();
	taskEXIT_CRITICAL(&myMutex);

     #endif

}
#endif