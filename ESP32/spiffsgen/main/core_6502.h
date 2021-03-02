#pragma once

#include <stdint.h>

union word
{
	uint16_t value;

	struct
	{
		uint8_t lo;
		uint8_t hi;
	} s;
};

struct reg_flags
{
	uint8_t carry;
	uint8_t zero;
	uint8_t interrupt;
	uint8_t decimal;
	uint8_t break_flag;
	uint8_t always_one;
	uint8_t overflow;
	uint8_t negative;
};

struct mos6502_reg
{
	uint8_t               a;
	uint8_t               x;
	uint8_t               y;
	uint8_t              sp;
	union  word	         pc;
	struct reg_flags  flags;
	uint32_t		 clocks;
	uint32_t  sendBytes;
};

struct system;

typedef uint16_t (*opFkt)(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);

struct opcodes
{
	opFkt     fkt;
	uint32_t  len;
	uint32_t  cyles;
};

struct mos6502
{
	struct mos6502_reg reg;
	struct opcodes     opcode[256];
};

struct memorymap
{
	uint8_t mem[65536];
	uint8_t sendBuf[1024];

};

struct system
{
	struct mos6502   cpu;
	struct memorymap bus;
};

void init_6502_sytem(struct system *system);
void tick_6502_system(struct system* system);

