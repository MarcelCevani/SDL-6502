#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "SDL.h"
#include "core_6502.h"


// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define WINDOW_WIDTH 640 
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1234"

SDL_Color colorTable[16];
BOOL readKeyboard(struct system* system, SDL_Event& event);
void write_emu_u8(SOCKET s, uint16_t adr, uint8_t value);
void usleep(__int64 usec);
SOCKET server_socket;
SOCKET s;
#define LOW_BYTE(x)     ((unsigned char)((x)&0xFF))
#define HIGH_BYTE(x)    ((unsigned char)(((x)>>8)&0xFF))

int main(int argc, char* argv[])
{
    SDL_Event event;
    SDL_Renderer* render;
    SDL_Window* window;
	WSADATA wsaData;
	int iResult;

	WSADATA wsa;
	SOCKET new_socket;
	struct sockaddr_in server, client;
	int c;
	char* message;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
    int i;

    SDL_Init(SDL_INIT_VIDEO);

    /* Create a Window */
    window = SDL_CreateWindow("6502 Emulator", 100, 100, 320, 320, SDL_WINDOW_SHOWN);
    /* Create a Render */
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
	struct system system_6502;

	colorTable[0].r = 0;
	colorTable[0].g = 0;
	colorTable[0].b = 0;

	colorTable[1].r = 0xff;
	colorTable[1].g = 0xff;
	colorTable[1].b = 0xff;

	colorTable[2].r = 0x88;
	colorTable[2].g = 0x00;
	colorTable[2].b = 0x00;

	colorTable[3].r = 0xaa;
	colorTable[3].g = 0xff;
	colorTable[3].b = 0xee;

	colorTable[4].r = 0xcc;
	colorTable[4].g = 0x44;
	colorTable[4].b = 0xcc;

	colorTable[5].r = 0x00;
	colorTable[5].g = 0xcc;
	colorTable[5].b = 0x55;

	colorTable[6].r = 0x00;
	colorTable[6].g = 0x00;
	colorTable[6].b = 0xaa;

	colorTable[7].r = 0xee;
	colorTable[7].g = 0xee;
	colorTable[7].b = 0x77;

	colorTable[8].r = 0xdd;
	colorTable[8].g = 0x88;
	colorTable[8].b = 0x55;

	colorTable[9].r = 0x66;
	colorTable[9].g = 0x44;
	colorTable[9].b = 0x00;

	colorTable[10].r = 0xff;
	colorTable[10].g = 0x77;
	colorTable[10].b = 0x77;

	colorTable[11].r = 0x33;
	colorTable[11].g = 0x33;
	colorTable[11].b = 0x33;

	colorTable[12].r = 0x77;
	colorTable[12].g = 0x77;
	colorTable[12].b = 0x77;

	colorTable[13].r = 0xaa;
	colorTable[13].g = 0xff;
	colorTable[13].b = 0x66;

	colorTable[14].r = 0x00;
	colorTable[14].g = 0x88;
	colorTable[14].b = 0xff;

	colorTable[15].r = 0xbb;
	colorTable[15].g = 0xbb;
	colorTable[15].b = 0xbb;

	init_6502_sytem(&system_6502);
	

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	//Create a socket
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(1234);

	//Bind
	if (bind(server_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
	}

	//Listen
	listen(server_socket, 3);


	c = sizeof(struct sockaddr_in);
	s = accept(server_socket, (struct sockaddr*)&client, &c);
	
	if (s == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
	}


	int nodelay = 1;
	setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (const char*)&nodelay, sizeof(int));

	puts("Connection accepted");

	uint8_t borderColor = 0x00;

	uint16_t scanline = 0;

	system_6502.cpu.reg.clocks = 0;

	BOOL bHitBreakPoint = FALSE;

	int count = 0;

	while(1)
	{
	
#if 0
		if(bHitBreakPoint == TRUE)
		{
			if (readKeyboard(&system_6502, event) == TRUE)
			{
				tick_6502_system(&system_6502);

				char* pDebug = &system_6502.cpu.opcode[system_6502.bus.mem[system_6502.cpu.reg.pc.value]].debug[0];

				printf("PC:%04x OP:%02x MEMONIC:%s | AR:%02x XR:%02x YR:%02x SP:%02x \n", system_6502.cpu.reg.pc.value,
					system_6502.bus.mem[system_6502.cpu.reg.pc.value],
					pDebug,
					system_6502.cpu.reg.a,
					system_6502.cpu.reg.x,
					system_6502.cpu.reg.y,
					system_6502.cpu.reg.sp);

				//printf("PC: %d OP:%s\n", system_6502.cpu.reg.pc.value, system_6502.cpu.opcode[system_6502.cpu.reg.pc.value].debug);
			}
		}
		else
		{
			tick_6502_system(&system_6502);

			if (system_6502.cpu.reg.pc.value == 0xB06A)
			{
				bHitBreakPoint = TRUE;
				char* pDebug = &system_6502.cpu.opcode[system_6502.bus.mem[system_6502.cpu.reg.pc.value]].debug[0];

				printf("PC:%04x OP:%02x MEMONIC:%s | AR:%02x XR:%02x YR:%02x SP:%02x \n", system_6502.cpu.reg.pc.value,
					system_6502.bus.mem[system_6502.cpu.reg.pc.value],
					pDebug,
					system_6502.cpu.reg.a,
					system_6502.cpu.reg.x,
					system_6502.cpu.reg.y,
					system_6502.cpu.reg.sp);
			}

		}

		scanline = system_6502.cpu.reg.clocks / 63;
		system_6502.bus.mem[0xD011] = HIGH_BYTE(scanline) & 0x80;
		system_6502.bus.mem[0xD012] = LOW_BYTE(scanline);
#endif


#if 1
		system_6502.cpu.reg.clocks = 0;

		while(system_6502.cpu.reg.clocks < 19656)
		{
			scanline = system_6502.cpu.reg.clocks / 63;
			system_6502.bus.mem[0xD011] = HIGH_BYTE(scanline) & 0x80;
			system_6502.bus.mem[0xD012] = LOW_BYTE(scanline);

			
			tick_6502_system(&system_6502);
			
#if 0
			if (system_6502.cpu.reg.pc.value == 0x0209)
			{
				int alarm = 0;
			}
#endif

#if 0
			if (count >= 40000)
			{
				count = 0;
				printf("PC:%04x\n", system_6502.cpu.reg.pc.value);
			}
#endif
#if 0
			scanline = system_6502.cpu.reg.clocks / 63;
			system_6502.bus.mem[0xD011] = HIGH_BYTE(scanline) & 0x80;
			system_6502.bus.mem[0xD012] = LOW_BYTE(scanline);
#endif
		}
#endif
		//Rendering
		//Sleep(20);
		Sleep(10);

		//SDL_RenderClear(render);
		//SDL_RenderPresent(render);
#if 0
		SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
		SDL_RenderClear(render);

		int x, y;


		for (y = 0; y < 320; y++)
			for (x = 0; x < 320; x++)
			{
				SDL_Color color = colorTable[system_6502.bus.mem[((y / 10) * 32 + (x / 10)) + 0x200] & 0x0F];
				SDL_SetRenderDrawColor(render, color.r, color.g, color.b, 255);
				SDL_RenderDrawPoint(render, x, y);
			}

		SDL_RenderPresent(render);
#endif


#if 0
		SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
		SDL_RenderClear(render);

		int x, y;

		for (y = 0; y < 320; y++)
		for (x = 0; x < 320; x++)
		{
			SDL_Color color = colorTable[system_6502.bus.mem[((y / 10) * 32 + (x / 10)) + 0x200] & 0x0F];
			SDL_SetRenderDrawColor(render, color.r, color.g, color.b, 255);
			SDL_RenderDrawPoint(render, x, y);
		}
		
		SDL_RenderPresent(render);
#endif
		
		//write_emu_u8(&s, 0xD020, borderColor);
		//write_emu_u8(&s, 0xD021, borderColor);


#if 0
		unsigned char buf = 'A';

		send(s, (const char*) &buf, 1, 0);
#endif
    }

	SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();








#if 0

	COLORREF colorTable[16];



	colorTable[0] = RGB(0, 0, 0);
	colorTable[1] = RGB(0xff, 0xff, 0xff);
	colorTable[2] = RGB(0x88, 0, 0);
	colorTable[3] = RGB(0xaa, 0xff, 0xee);
	colorTable[4] = RGB(0xcc, 0x44, 0xcc);
	colorTable[5] = RGB(0x00, 0xcc, 0x55);
	colorTable[6] = RGB(0x00, 0x00, 0xaa);
	colorTable[7] = RGB(0xee, 0xee, 0x77);
	colorTable[8] = RGB(0xdd, 0x88, 0x55);
	colorTable[9] = RGB(0x66, 0x44, 0x00);
	colorTable[10] = RGB(0xff, 0x77, 0x77);
	colorTable[11] = RGB(0x33, 0x33, 0x33);
	colorTable[12] = RGB(0x77, 0x77, 0x77);
	colorTable[13] = RGB(0xaa, 0xff, 0x66);
	colorTable[14] = RGB(0x00, 0x88, 0xff);
	colorTable[15] = RGB(0xbb, 0xbb, 0xbb);


	init_6502_sytem(&system_6502);
	//CDialogEx::OnOK();


	while (1)
	{
		long ticks = 800;

		while (ticks)
		{
			tick_6502_system(&system_6502);
			ticks--;
		}

		this->RedrawWindow();
	}



#endif
    return 0;
}

BOOL readKeyboard(struct system *system, SDL_Event &event)
{
	while(SDL_PollEvent(&event))
	{
		/* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
		switch (event.type)
		{
			case SDL_KEYUP:
			if (event.key.keysym.scancode == SDL_SCANCODE_F10)
				return TRUE;
			break;
#if 0
			case SDL_KEYDOWN:
			//system->bus.mem[0xff] = event.key.keysym.sym;
			
				printf("KEY\n");
				if (event.key.keysym.sym == SDL_SCANCODE_F10)
					return TRUE;
				break;

			case SDL_KEYUP:
			//system->bus.mem[0xff] = 0x00;
			if (event.key.keysym.scancode == SDL_SCANCODE_F10)
				return TRUE;
#endif
			break;
		}
	}
}

void write_emu_u8(SOCKET s, uint16_t adr, uint8_t value)
{
	send(s, (const char*)&adr, 2, 0);
	send(s, (const char*)&value, 1, 0);
}