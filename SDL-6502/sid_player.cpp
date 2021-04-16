#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <memory.h>
#include <windows.h>
#include "sid_player.h"

#define LOW_BYTE(x)     ((unsigned char)((x)&0xFF))
#define HIGH_BYTE(x)    ((unsigned char)(((x)>>8)&0xFF))

#pragma pack(push, 1)
struct sid_header
{
	uint8_t  magicID[4];   /* 00000 */ // PSID,RSID
	uint16_t version;      /* 00004 */ // 0001, 0002, 0003 or 0004
	uint16_t dataOffset;   /* 00006 */ // data offset der 6502 Binärdaten
	uint16_t loadOffset;   /* 00008 */ // in welchen Speicherbereich laden
	uint16_t initAddress;  /* 0000A */
	uint16_t playAddress;  /* 0000C */
	uint16_t songs;		   /* 0000E */
	uint16_t startSong;	   /* 00010 */
	uint32_t speed;        /* 00012 */
	uint8_t  name[32];     /* 00016 */
	uint8_t  author[32];   /* 00036 */
	uint8_t  released[32]; /* 00056 */
};
#pragma pack(pop)

void init_sid_player(struct sid_player *player)
{
	player->currentSongNr = 0;
	player->vBlankInterruptTime = 0; // 50Hz
	init_6502_sytem(&player->system_6502);
}

uint8_t load_sid_file(struct sid_player* player, char* pFileName)
{
	struct sid_header header;

	uint32_t fileSize;
	uint8_t  word_raw[2];
	uint8_t  dword_raw[4];
	uint16_t player_offset;
	union word  word_packed;
	union dword dword_packed;
	
	FILE* pSidFile;
	errno_t err;

	err = fopen_s(&pSidFile, pFileName, "rb");

	if(err == 0)
		printf("The SID-File was opened\n");

	if(pSidFile == NULL)
		return 1;
	
	fseek(pSidFile, 0, SEEK_END);
	fileSize = ftell(pSidFile);
	fseek(pSidFile, 0 , SEEK_SET);

	memset(&player->system_6502.bus.mem[0], 0x00, 65536);

	fread(&header.magicID[0], 4, 1, pSidFile);
	
	fread(&word_raw[0], sizeof(word_raw), 1, pSidFile);
	word_packed.s.lo = word_raw[1];
	word_packed.s.hi = word_raw[0];
	header.version = word_packed.value;

	fread(&word_raw[0], sizeof(word_raw), 1, pSidFile);
	word_packed.s.lo = word_raw[1];
	word_packed.s.hi = word_raw[0];
	header.dataOffset = word_packed.value;

	fread(&word_raw[0], sizeof(word_raw), 1, pSidFile);
	word_packed.s.lo = word_raw[1];
	word_packed.s.hi = word_raw[0];
	header.loadOffset = word_packed.value;

	fread(&word_raw[0], sizeof(word_raw), 1, pSidFile);
	word_packed.s.lo = word_raw[1];
	word_packed.s.hi = word_raw[0];
	header.initAddress = word_packed.value;

	fread(&word_raw[0], sizeof(word_raw), 1, pSidFile);
	word_packed.s.lo = word_raw[1];
	word_packed.s.hi = word_raw[0];
	header.playAddress = word_packed.value;

	fread(&word_raw[0], sizeof(word_raw), 1, pSidFile);
	word_packed.s.lo = word_raw[1];
	word_packed.s.hi = word_raw[0];
	header.songs = word_packed.value;

	fread(&word_raw[0], sizeof(word_raw), 1, pSidFile);
	word_packed.s.lo = word_raw[1];
	word_packed.s.hi = word_raw[0];
	header.startSong = word_packed.value;

	fread(&dword_raw[0], sizeof(dword_raw), 1, pSidFile);
	dword_packed.s.b1 = dword_raw[3];
	dword_packed.s.b2 = dword_raw[2];
	dword_packed.s.b3 = dword_raw[1];
	dword_packed.s.b4 = dword_raw[0];
	header.speed = dword_packed.value;

	fread(&header.name[0], sizeof(header.name), 1, pSidFile);
	fread(&header.author[0], sizeof(header.author), 1, pSidFile);
	fread(&header.released[0], sizeof(header.released), 1, pSidFile);
	
	fseek(pSidFile, header.dataOffset, SEEK_SET);
		
	fileSize = fileSize - header.dataOffset;

	if(header.loadOffset == 0x0000)
	{
		fread(&word_raw[0], sizeof(word_raw), 1, pSidFile);
		word_packed.s.lo = word_raw[0];
		word_packed.s.hi = word_raw[1];
		header.loadOffset = word_packed.value;
		fileSize = fileSize - 2;
	}

	fread(&player->system_6502.bus.mem[header.loadOffset], fileSize, 1, pSidFile);

	fclose(pSidFile);

	if (header.playAddress)
	{
		/* Rasterline player */
		player_offset = header.loadOffset - 18;

		player->system_6502.bus.mem[player_offset + 0] = 0xa9; // LDA
		player->system_6502.bus.mem[player_offset + 1] = 0x00; // Songnummer
		player->system_6502.bus.mem[player_offset + 2] = 0x20; // JSR
		player->system_6502.bus.mem[player_offset + 3] = LOW_BYTE(header.initAddress);
		player->system_6502.bus.mem[player_offset + 4] = HIGH_BYTE(header.initAddress);
		player->system_6502.bus.mem[player_offset + 5] = 0xa9; // LDA
		player->system_6502.bus.mem[player_offset + 6] = 0x64; // Rasterline = 0
		player->system_6502.bus.mem[player_offset + 7] = 0xcd; // CMP
		player->system_6502.bus.mem[player_offset + 8] = LOW_BYTE(0xd012);  // aktuelle Rasteline
		player->system_6502.bus.mem[player_offset + 9] = HIGH_BYTE(0xd012); // aktuelle Rasteline
		player->system_6502.bus.mem[player_offset + 10] = 0xd0; // BNE
		player->system_6502.bus.mem[player_offset + 11] = 0xf9; // -3
		player->system_6502.bus.mem[player_offset + 12] = 0x20; // JSR
		player->system_6502.bus.mem[player_offset + 13] = LOW_BYTE(header.playAddress);
		player->system_6502.bus.mem[player_offset + 14] = HIGH_BYTE(header.playAddress);
		player->system_6502.bus.mem[player_offset + 15] = 0x4c; // JMP
		player->system_6502.bus.mem[player_offset + 16] = LOW_BYTE(player_offset + 5);
		player->system_6502.bus.mem[player_offset + 17] = HIGH_BYTE(player_offset + 5);
	}
	else
	{
		/* IRQ/CIA player */
		player_offset = header.loadOffset - 9;

		player->system_6502.bus.mem[player_offset + 0] = 0xa9; // LDA
		player->system_6502.bus.mem[player_offset + 1] = 0x00; // Songnummer
		player->system_6502.bus.mem[player_offset + 2] = 0x20; // JSR
		player->system_6502.bus.mem[player_offset + 3] = LOW_BYTE(header.initAddress); // initAdresse LO
		player->system_6502.bus.mem[player_offset + 4] = HIGH_BYTE(header.initAddress); // initAdresse HI
		player->system_6502.bus.mem[player_offset + 5] = 0x4c; // JMP
		player->system_6502.bus.mem[player_offset + 7] = LOW_BYTE(player_offset + 5);
		player->system_6502.bus.mem[player_offset + 8] = HIGH_BYTE(player_offset + 5);
	}

	player->system_6502.bus.mem[0x01] = 0x37;
	player->system_6502.bus.mem[0xfffe] = 0x48;
	player->system_6502.bus.mem[0xffff] = 0xff;
	
	player->system_6502.bus.mem[0xff48] = 0x48; // PHA
	player->system_6502.bus.mem[0xff49] = 0x8a; // TXA
	player->system_6502.bus.mem[0xff4a] = 0x48; // PHA
	player->system_6502.bus.mem[0xff4b] = 0x98; // TYA
	player->system_6502.bus.mem[0xff4c] = 0x48; // PHA
	player->system_6502.bus.mem[0xff4d] = 0xba; // TSX
	player->system_6502.bus.mem[0xff4e] = 0xbd; // LDA $0104,X
	player->system_6502.bus.mem[0xff4f] = 0x04;  
	player->system_6502.bus.mem[0xff50] = 0x01;
	player->system_6502.bus.mem[0xff51] = 0x29; // AND #$10
	player->system_6502.bus.mem[0xff52] = 0x10;
	player->system_6502.bus.mem[0xff53] = 0xf0; // BEQ $FF58
	player->system_6502.bus.mem[0xff54] = 0x03; 
	player->system_6502.bus.mem[0xff55] = 0x6c; // JMP ($0316)
	player->system_6502.bus.mem[0xff56] = 0x16;
	player->system_6502.bus.mem[0xff57] = 0x03;
	player->system_6502.bus.mem[0xff58] = 0x6c; // JMP ($0314)
	player->system_6502.bus.mem[0xff59] = 0x14;
	player->system_6502.bus.mem[0xff5a] = 0x03;
	
	player->playerOffset = player_offset;

	// TODO: Fehlercode
	return 0;
}

uint8_t load_sid_from_memory(struct sid_player* player, uint32_t memorySize, uint8_t *sid_memory)
{
	struct sid_header header;
	uint8_t  word_raw[2];
	uint8_t  dword_raw[4];
	uint16_t player_offset;
	uint32_t read_offset;
	uint32_t data_size;

	union word  word_packed;
	union dword dword_packed;

	read_offset = 0;

	memset(&player->system_6502.bus.mem[0], 0x00, 65536);

	memcpy(&header.magicID[0], &sid_memory[read_offset], 4);
	read_offset += 4;

	memcpy(&word_raw[0], &sid_memory[read_offset], sizeof(word_raw));
	read_offset += sizeof(word_raw);
	
	word_packed.s.lo = word_raw[1];
	word_packed.s.hi = word_raw[0];
	header.version = word_packed.value;

	memcpy(&word_raw[0], &sid_memory[read_offset], sizeof(word_raw));
	read_offset += sizeof(word_raw);
	
	word_packed.s.lo = word_raw[1];
	word_packed.s.hi = word_raw[0];
	header.dataOffset = word_packed.value;

	memcpy(&word_raw[0], &sid_memory[read_offset], sizeof(word_raw));
	read_offset += sizeof(word_raw);

	word_packed.s.lo = word_raw[1];
	word_packed.s.hi = word_raw[0];
	header.loadOffset = word_packed.value;

	memcpy(&word_raw[0], &sid_memory[read_offset], sizeof(word_raw));
	read_offset += sizeof(word_raw);

	word_packed.s.lo = word_raw[1];
	word_packed.s.hi = word_raw[0];
	header.initAddress = word_packed.value;

	memcpy(&word_raw[0], &sid_memory[read_offset], sizeof(word_raw));
	read_offset += sizeof(word_raw);

	word_packed.s.lo = word_raw[1];
	word_packed.s.hi = word_raw[0];
	header.playAddress = word_packed.value;

	memcpy(&word_raw[0], &sid_memory[read_offset], sizeof(word_raw));
	read_offset += sizeof(word_raw);
	
	word_packed.s.lo = word_raw[1];
	word_packed.s.hi = word_raw[0];
	header.songs = word_packed.value;

	memcpy(&word_raw[0], &sid_memory[read_offset], sizeof(word_raw));
	read_offset += sizeof(word_raw);

	word_packed.s.lo = word_raw[1];
	word_packed.s.hi = word_raw[0];
	header.startSong = word_packed.value;

	memcpy(&dword_raw[0], &sid_memory[read_offset], sizeof(dword_raw));
	read_offset += sizeof(dword_raw);
	
	dword_packed.s.b1 = dword_raw[3];
	dword_packed.s.b2 = dword_raw[2];
	dword_packed.s.b3 = dword_raw[1];
	dword_packed.s.b4 = dword_raw[0];
	header.speed = dword_packed.value;

	memcpy(&header.name[0], &sid_memory[read_offset], 32);
	read_offset += 32;

	memcpy(&header.author[0], &sid_memory[read_offset], 32);
	read_offset += 32;

	memcpy(&header.released[0], &sid_memory[read_offset], 32);
	read_offset += 32;

	read_offset = header.dataOffset;

	data_size = memorySize - header.dataOffset;

	if (header.loadOffset == 0x0000)
	{
		memcpy(&word_raw[0], &sid_memory[read_offset], sizeof(word_raw));
		read_offset += sizeof(word_raw);

		word_packed.s.lo = word_raw[0];
		word_packed.s.hi = word_raw[1];

		header.loadOffset = word_packed.value;

		data_size = data_size - 2;
	}

	memcpy(&player->system_6502.bus.mem[header.loadOffset], &sid_memory[read_offset], data_size);

	if (header.playAddress)
	{
		/* Rasterline player */
		player_offset = header.loadOffset - 18;

		player->system_6502.bus.mem[player_offset + 0] = 0xa9; // LDA
		player->system_6502.bus.mem[player_offset + 1] = 0x00; // Songnummer
		player->system_6502.bus.mem[player_offset + 2] = 0x20; // JSR
		player->system_6502.bus.mem[player_offset + 3] = LOW_BYTE(header.initAddress);
		player->system_6502.bus.mem[player_offset + 4] = HIGH_BYTE(header.initAddress);
		player->system_6502.bus.mem[player_offset + 5] = 0xa9; // LDA
		player->system_6502.bus.mem[player_offset + 6] = 0x64; // Rasterline = 0
		player->system_6502.bus.mem[player_offset + 7] = 0xcd; // CMP
		player->system_6502.bus.mem[player_offset + 8] = LOW_BYTE(0xd012);  // aktuelle Rasteline
		player->system_6502.bus.mem[player_offset + 9] = HIGH_BYTE(0xd012); // aktuelle Rasteline
		player->system_6502.bus.mem[player_offset + 10] = 0xd0; // BNE
		player->system_6502.bus.mem[player_offset + 11] = 0xf9; // -3
		player->system_6502.bus.mem[player_offset + 12] = 0x20; // JSR
		player->system_6502.bus.mem[player_offset + 13] = LOW_BYTE(header.playAddress);
		player->system_6502.bus.mem[player_offset + 14] = HIGH_BYTE(header.playAddress);
		player->system_6502.bus.mem[player_offset + 15] = 0x4c; // JMP
		player->system_6502.bus.mem[player_offset + 16] = LOW_BYTE(player_offset + 5);
		player->system_6502.bus.mem[player_offset + 17] = HIGH_BYTE(player_offset + 5);
	}
	else
	{
		/* IRQ/CIA player */
		player_offset = header.loadOffset - 9;

		player->system_6502.bus.mem[player_offset + 0] = 0xa9; // LDA
		player->system_6502.bus.mem[player_offset + 1] = 0x00; // Songnummer
		player->system_6502.bus.mem[player_offset + 2] = 0x20; // JSR
		player->system_6502.bus.mem[player_offset + 3] = LOW_BYTE(header.initAddress); // initAdresse LO
		player->system_6502.bus.mem[player_offset + 4] = HIGH_BYTE(header.initAddress); // initAdresse HI
		player->system_6502.bus.mem[player_offset + 5] = 0x4c; // JMP
		player->system_6502.bus.mem[player_offset + 7] = LOW_BYTE(player_offset + 5);
		player->system_6502.bus.mem[player_offset + 8] = HIGH_BYTE(player_offset + 5);
	}

	player->system_6502.bus.mem[0x01] = 0x37;
	player->system_6502.bus.mem[0xfffe] = 0x48;
	player->system_6502.bus.mem[0xffff] = 0xff;

	player->system_6502.bus.mem[0xff48] = 0x48; // PHA
	player->system_6502.bus.mem[0xff49] = 0x8a; // TXA
	player->system_6502.bus.mem[0xff4a] = 0x48; // PHA
	player->system_6502.bus.mem[0xff4b] = 0x98; // TYA
	player->system_6502.bus.mem[0xff4c] = 0x48; // PHA
	player->system_6502.bus.mem[0xff4d] = 0xba; // TSX
	player->system_6502.bus.mem[0xff4e] = 0xbd; // LDA $0104,X
	player->system_6502.bus.mem[0xff4f] = 0x04;
	player->system_6502.bus.mem[0xff50] = 0x01;
	player->system_6502.bus.mem[0xff51] = 0x29; // AND #$10
	player->system_6502.bus.mem[0xff52] = 0x10;
	player->system_6502.bus.mem[0xff53] = 0xf0; // BEQ $FF58
	player->system_6502.bus.mem[0xff54] = 0x03;
	player->system_6502.bus.mem[0xff55] = 0x6c; // JMP ($0316)
	player->system_6502.bus.mem[0xff56] = 0x16;
	player->system_6502.bus.mem[0xff57] = 0x03;
	player->system_6502.bus.mem[0xff58] = 0x6c; // JMP ($0314)
	player->system_6502.bus.mem[0xff59] = 0x14;
	player->system_6502.bus.mem[0xff5a] = 0x03;

	player->playerOffset = player_offset;

	// TODO: Fehlercode
	return 0;
}

// Gets the current number of ticks from QueryPerformanceCounter. Throws an
// exception if the call to QueryPerformanceCounter fails.
static inline int64_t GetTicks()
{
	LARGE_INTEGER ticks;
	if (!QueryPerformanceCounter(&ticks))
	{
		
	}
	return ticks.QuadPart;
}

uint8_t init_sid_tune(struct sid_player* player, uint8_t tune_nr)
{
	player->currentSongNr = tune_nr;
	player->system_6502.bus.mem[player->playerOffset + 1] = tune_nr;
	player->system_6502.cpu.reg.pc.value = player->playerOffset;
	return 0;
}

uint8_t play_sid_tune(struct sid_player* player, uint32_t max_cyles)
{
	player->system_6502.cpu.reg.clocks = 0;
	
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

	return 0;
}
