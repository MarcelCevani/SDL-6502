/* SPIFFS Image Generation on Build Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

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
#include "core_6502.h"
#include <lwip/sockets.h>

static const char *TAG = "SIDBOY";

#define LOW_BYTE(x)     ((unsigned char)((x)&0xFF))
#define HIGH_BYTE(x)    ((unsigned char)(((x)>>8)&0xFF))

struct system system_6502;


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
int s;

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

void app_main(void)
{
    ESP_LOGI(TAG, "Initializing SPIFFS");
    esp_err_t ret;
    
   // struct sockaddr_in server;


    //Initialize NVS
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = false
    };

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }


    // GPIO setup

    gpio_pad_select_gpio(22); // D0
    gpio_pad_select_gpio(23); // CLK
    gpio_pad_select_gpio(21); // D7
    gpio_pad_select_gpio(19); // CS
    gpio_pad_select_gpio(18); // A0
    gpio_pad_select_gpio(5);  // A1
    gpio_pad_select_gpio(15);  // A2
    gpio_pad_select_gpio(13);  // A3
    gpio_pad_select_gpio(4);  // A4
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
   
    ledc_timer_config_t ledc_timer;

  ledc_timer.clk_cfg = LEDC_USE_APB_CLK;

  ledc_timer.speed_mode = LEDC_HIGH_SPEED_MODE;       // timer mode
  ledc_timer.duty_resolution = LEDC_TIMER_2_BIT;      // resolution of PWM duty
  ledc_timer.timer_num = LEDC_TIMER_0;                // timer index
  ledc_timer.freq_hz = 1000000;                       // frequency of PWM signal
  // Set configuration of timer0 for high speed channels
  esp_err_t result = ledc_timer_config(&ledc_timer);
  if (result == ESP_OK)
        printf("frequency: %d", ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0));
  ledc_channel_config_t ledc_channel = {
          .gpio_num   = LEDC_0_PIN,
          .speed_mode = LEDC_HIGH_SPEED_MODE,
          .channel    = LEDC_CHANNEL_0,
          .intr_type  = LEDC_INTR_DISABLE,
          .timer_sel  = LEDC_TIMER_0,
           .duty       = 2,
          //.duty       = 31,
          .hpoint     = 0
  };

   // Set LED Controller with previously prepared configuration
   ledc_channel_config(&ledc_channel);









    init_6502_sytem(&system_6502);
    
    FILE* pRomFile;
    pRomFile = fopen("/spiffs/memory.bin", "rb");
    fread(&system_6502.bus.mem[0x0000], 0xffff, 1, pRomFile);
    fclose(pRomFile);
    
    uint16_t scanline = 0;




     gpio_set_level(22, 1); // RES deaktivieren
    gpio_set_level(19, 1); // CS


#if 0
    wifi_init_sta();

    while(!flag_wifi_connected)
    {
        usleep(1000 * 20);
    }

    printf("Bin verbunden!\n");
#endif

#if 0
    //Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) < 0)
	{
		printf("Could not create socket\n");
        
        while(1)
            usleep(1000 * 20);

	}

    printf("Socket created.\n");
    server.sin_addr.s_addr = inet_addr("192.168.0.217");
  	server.sin_family = AF_INET;
	server.sin_port = htons(1234);

	//Connect to remote server
	int fehler = connect(s, (struct sockaddr*)&server, sizeof(struct sockaddr_in6));

    if (fehler < 0)
	{
		printf("Connect fehler = %d\n", errno);

        while(1)
            usleep(1000 * 20);
	}

    #endif


#if 0    
        int addr_family = AF_INET;
        int ip_protocol = 0;
        struct sockaddr_in dest_addr;
        struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
        dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr_ip4->sin_family = AF_INET;
        dest_addr_ip4->sin_port = htons(1234);
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
        ESP_LOGI(TAG, "Socket bound, port %d", 1234);
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
        s = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (s < 0) {
                ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
                return;
        }


        int nodelay = 1;
        setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (void *)&nodelay, sizeof(int));

        printf("Free ram:%d\n", xPortGetFreeHeapSize());
#endif

#if 0
        int nodelay = 1;
        setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (void *)&nodelay, sizeof(int));
#endif

       // err = fcntl (s, F_SETFL , O_NONBLOCK );

    #if 0
printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(1234);

	//Connect to remote server
	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}
#endif

	gpio_set_level(19, 1); // CS
    srand(43434);

    printf("Starte wiedergabe\n");

    while(1)
    {
        system_6502.cpu.reg.clocks = 0;
        system_6502.cpu.reg.sendBytes = 0;

        unsigned long currentMillis1 = xTaskGetTickCount();
        unsigned long test = 0;

		while(system_6502.cpu.reg.clocks < 19656)
	    {
	    	tick_6502_system(&system_6502);
            scanline = system_6502.cpu.reg.clocks / 63;
		    system_6502.bus.mem[0xD011] = HIGH_BYTE(scanline) & 0x80;
			system_6502.bus.mem[0xD012] = LOW_BYTE(scanline);
        }
      
        send(s, (const char*)&system_6502.bus.sendBuf[0], system_6502.cpu.reg.sendBytes, 0);

     

         unsigned long currentMillis2 = xTaskGetTickCount();

         //printf("ticks = %lu, %d\n",currentMillis2-currentMillis1,  system_6502.cpu.reg.sendBytes );
        // usleep(1000 * 16);
        usleep(1000 * 10);
    
    }

    // All done, unmount partition and disable SPIFFS
    esp_vfs_spiffs_unregister(NULL);
    ESP_LOGI(TAG, "SPIFFS unmounted");
}
