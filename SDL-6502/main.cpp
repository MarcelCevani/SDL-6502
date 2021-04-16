#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "sid_player.h"


// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define WINDOW_WIDTH 640 
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1234"

void write_emu_u8(SOCKET s, uint16_t adr, uint8_t value);
void usleep(__int64 usec);

SOCKET server_socket;
SOCKET s;

SOCKET player_server_socket;
SOCKET player_socket;

#define ZST_PLAYER_NO_SONG       0
#define ZST_PLAYER_CHECK_CMD     1
#define ZST_PLAYER_LOAD_TUNE     2
#define ZST_PLAYER_PLAY_TUNE     3

uint8_t sid_memory[65536];

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	int iResult;

	WSADATA wsa;
	SOCKET new_socket;
	struct sockaddr_in server, client, server2;
	int c;
	char* message;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
    int i;

#if 1
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

	c = sizeof(struct sockaddr);
	s = accept(server_socket, (struct sockaddr*)&client, &c);
	
	if (s == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
	}


	int nodelay = 1;
	setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (const char*)&nodelay, sizeof(int));

	puts("Connection accepted from MAME");

	uint8_t borderColor = 0x00;

	uint16_t scanline = 0;
#endif




	//Create a socket
	if ((player_server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server2.sin_family = AF_INET;
	server2.sin_addr.s_addr = INADDR_ANY;
	server2.sin_port = htons(4321);

	//Bind
	if (bind(player_server_socket, (struct sockaddr*)&server2, sizeof(server2)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
	}

	//Listen
	listen(player_server_socket, 3);

	c = sizeof(struct sockaddr);
	player_socket = accept(player_server_socket, (struct sockaddr*)&server2, &c);

	if (player_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
	}

	puts("Connection accepted from SIDBoy player");

	BOOL bHitBreakPoint = FALSE;

	int count = 0;

	struct sid_player player;

	init_sid_player(&player);

	uint32_t player_state = ZST_PLAYER_NO_SONG;
	u_long bytes_available;
	uint8_t cmd_buffer[5];

	while(1)
	{
		switch(player_state)
		{
			case ZST_PLAYER_NO_SONG:
			{	
				ioctlsocket(player_socket, FIONREAD, &bytes_available);


				int value;
				error = ioctl(sock, FIONREAD, &value);

				if(bytes_available >= 5)
				{
					player_state = ZST_PLAYER_CHECK_CMD;
					break;
				}

				Sleep(100);
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
			
				load_sid_from_memory(&player, filesize.value, &sid_memory[0]);
				init_sid_tune(&player, 0);

				player_state = ZST_PLAYER_PLAY_TUNE;

				break;
			}

			case ZST_PLAYER_PLAY_TUNE:
			{
				play_sid_tune(&player, 19656);
	
				ioctlsocket(player_socket, FIONREAD, &bytes_available);

				if (bytes_available != 0)
				{
					player_state = ZST_PLAYER_CHECK_CMD;
					break;
				}

				break;
			}
		}

		//play_sid_tune(&player, 19656);
		//play_sid_tune(&player, 19656);
		//Sleep(14);
    }

	return 0;
}

void write_emu_u8(SOCKET s, uint8_t adr, uint8_t value)
{
	send(s, (const char*)&adr, 1, 0);
	send(s, (const char*)&value, 1, 0);
}