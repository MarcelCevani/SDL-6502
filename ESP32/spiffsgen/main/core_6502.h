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

union dword
{
	uint32_t value;

	struct
	{
		uint8_t b1;
		uint8_t b2;
		uint8_t b3;
		uint8_t b4;
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
	uint32_t         sendBytes;
	uint32_t         start_time;
	uint32_t		 anz_PlayJSR;
	uint8_t		 opcode_long;
	
		uint32_t		 max_cyles;
	uint32_t         extra_cyle;
};

struct system;

typedef uint16_t (*opFkt)(struct system *system, uint8_t *opPara, uint32_t len);

struct opcodes
{
	opFkt     fkt;
	uint32_t  len;
	uint32_t  cyles;
};

struct cia2
{
	int32_t timerA_latch;
	int32_t timerA_count;
	int32_t timerB_latch;
	int32_t timerB_count;
};

struct mos6502
{
	struct mos6502_reg reg;
	struct opcodes     opcode[256];
};

struct memorymap
{
	uint8_t mem[65536];
};

struct system
{
	struct mos6502   cpu;
	struct memorymap bus;
	struct cia2      cia2;
};

void init_6502_sytem(struct system *system);
uint32_t tick_6502_system(struct system* system);
void generate_irq_6502_system(struct system* system);
void generate_mni_6502_system(struct system* system);