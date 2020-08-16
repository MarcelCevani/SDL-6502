#include "core_6502.h"

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <memory.h>
#include <time.h>

uint16_t inst_LDA_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDA_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDA_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDA_Z(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDA_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STA_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDA_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_INC_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDA_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STA_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STA_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDX_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDY_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDX_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STA_Z(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STX_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STY_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STA_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STA_IY(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);
uint16_t inst_AND_I(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CLC_IM(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);
uint16_t inst_SEC_IM(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ADC_I(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ADC_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ADC_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDY_I(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);
uint16_t inst_JMP_A(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STA_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDA_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_INC_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_DEC_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CMP_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CMP_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_CMP_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_CMY_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CMX_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_BNE(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_BCC(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_BPL(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_BMI(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_BEQ(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_TAX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_TXA(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_TYA(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_TAY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_INY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_JSR(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_RTS(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_DEX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_INX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_DEY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ASL(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LSR(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ROL(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ROR_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_SBC_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_SBC_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_SBC_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ORA_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_EOR_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_AND_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_NOP(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_PHA(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_PLA(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ADC_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_unknown(struct system* system, uint8_t *opPara, uint32_t len, uint32_t cyles);

void init_6502_sytem(struct system *system)
{
	uint32_t i;
	uint32_t rom_start = 0x600;

	//uint8_t  opByte;

	memset(system, 0x00, sizeof(struct system));

	for (i = 0; i < 256; i++)
		system->cpu.opcode[i].fkt = inst_unknown;

	// ORA (Zero Page) 0x05 LEN 2 TIM 3
	system->cpu.opcode[0x05] = { inst_ORA_Z, 2, 3 };
	// LDY  (Immidiate) 0xA0 LEN 2 TIM 2			  // ok?
	system->cpu.opcode[0xA0] = { inst_LDY_I, 2, 2 };
	// CLC  (Implied)   0x18 LEN 1 TIM 2			  // ok?
	system->cpu.opcode[0x18] = { inst_CLC_IM, 1, 2 };
	// AND  (Immidiate) 0x29 LEN 2 TIM 2
	system->cpu.opcode[0x29] = { inst_AND_I, 2, 2 };  // ok?
	// JMP  (Absolute) 0x4C LEN 3 TIM 3
	system->cpu.opcode[0x4c] = { inst_JMP_A, 3, 3 };
	// ADC  (Immidiate) 0x69 LEN 2 TIM 3			  // Overflow TODO
	system->cpu.opcode[0x69] = { inst_ADC_I, 2, 3 };
	// ADC  (Zerro Page) 0x65 LEN 2 TIM 3			  // Overflow TODO
	system->cpu.opcode[0x65] = { inst_ADC_Z, 2, 3 };
	// ADC  (Absolute,X) 0x7d LEN 3 TIM 4			  // Overflow TODO
	system->cpu.opcode[0x7d] = { inst_ADC_AX, 3, 4 };
	// ADC  (Indirect,Y) 0x71 LEN 2 TIM 5
	system->cpu.opcode[0x71] = { inst_ADC_IY, 2, 5 };
	// LDA  (Indirect,X) 0xa1 LEN 2 TIM 6
	system->cpu.opcode[0xa1] = { inst_LDA_IX, 2, 2 }; // ok?
	// LDX  (Immidiate) 0xa2 LEN 2 TIM 3
	system->cpu.opcode[0xa2] = { inst_LDX_I, 2, 2 }; // ok?
	// LDA  (Zero Page) 0xa5 LEN 2 TIM 3
	system->cpu.opcode[0xa5] = { inst_LDA_Z, 2, 3 }; // ok?
	// LDX  (Zero Page) 0xa6 LEN 2 TIM 3
	system->cpu.opcode[0xa6] = { inst_LDX_Z, 2, 3 }; // ok?
	// DEC  (Zero Page) 0xC6 LEN 2 TIM 5
	system->cpu.opcode[0xC6] = { inst_DEC_Z, 2, 5 }; // ok?
	// LDA  (Immediate) 0xa9 LEN 2 TIM 2
	system->cpu.opcode[0xa9] = { inst_LDA_I, 2, 2 };
	// LDA  (Absolute,X) 0xbd LEN 3 TIM 4+
	system->cpu.opcode[0xbd] = { inst_LDA_AX, 3, 4 };
	// LDA  (Absolute,Y) 0xb9 LEN 3 TIM 4+
	system->cpu.opcode[0xb9] = { inst_LDA_AY, 3, 4 };
	// CMP  (Immediate) 0xc9 LEN 2 TIM 2
	system->cpu.opcode[0xc9] = { inst_CMP_I, 2, 2 };
	// CMP  (Zero Page) 0xc5 LEN 2 TIM 2
	system->cpu.opcode[0xc5] = { inst_CMP_Z, 2, 3 };
	// CMY  (Immediate) 0xc0 LEN 2 TIM 2
	system->cpu.opcode[0xc0] = { inst_CMY_I, 2, 2 };
	// CMX  (Immediate) 0xe0 LEN 2 TIM 2
	system->cpu.opcode[0xe0] = { inst_CMX_I, 2, 2 };
	// BNE  (Branch not Equal) 0xd0 LEN 2 TIM 2
	system->cpu.opcode[0xd0] = { inst_BNE, 2, 2 };
	// BMI  (Branch on MInus) 0x30 LEN 2 TIM 2
	system->cpu.opcode[0x30] = { inst_BMI, 2, 2 };
	// BEQ  (Branch On Equal) 0xf0 LEN 2 TIM 2
	system->cpu.opcode[0xf0] = { inst_BEQ, 2, 2 };

	// BCC  (Branch on Carry Clear) 0x90 LEN 2 TIM 2
	system->cpu.opcode[0x90] = { inst_BCC, 2, 2 };

	// STA  (Indirect, X) 0x81 LEN 2 TIM 6
	system->cpu.opcode[0x81] = { inst_STA_IX, 2, 6 }; // ok?
	// STA  (Zero Page) 0x85 LEN 2 TIM 3
	system->cpu.opcode[0x85] = { inst_STA_Z, 2, 3 }; // ok?
	// STA  (Immidiate Y) 0x91 LEN 2 TIM 6
	system->cpu.opcode[0x91] = { inst_STA_IY, 2, 6 }; // ok?
	// LDA  (Indirect, Y) 0xB1 LEN 2 TIM 5
	system->cpu.opcode[0xB1] = { inst_LDA_IY, 2, 5 }; // ok?
	// TAX  (Transfer A to X) 0xAA LEN 1 TIM 2
	system->cpu.opcode[0xAA] = { inst_TAX, 1, 2 }; // ok?
	// TXA  (Transfer X to A) 0x8A LEN 1 TIM 2
	system->cpu.opcode[0x8A] = { inst_TXA, 1, 2 }; // ok?
	// TYA  (Transfer Y to A) 0x98 LEN 1 TIM 2
	system->cpu.opcode[0x98] = { inst_TYA, 1, 2 }; // ok?
	// INY  (INcrement Y) 0xC8 LEN 1 TIM 2
	system->cpu.opcode[0xC8] = { inst_INY, 1, 2 }; // ok?
	// JSR  (Jump to SubRoutine) 0x20 LEN 3 TIM 2
	system->cpu.opcode[0x20] = { inst_JSR, 3, 6 }; // ok?
	// STY  (Zero Page) 0x84 LEN 2 TIM 3
	system->cpu.opcode[0x84] = { inst_STY_Z, 2, 3 };
	// STX  (Zero Page) 0x86 LEN 2 TIM 3
	system->cpu.opcode[0x86] = { inst_STX_Z, 2, 3 };
	// LDY  (Zero Page) 0xA4 LEN 2 TIM 3
	system->cpu.opcode[0xA4] = { inst_LDY_Z, 2, 3 };
	// RTS  (ReTurn from Subroutine) 0x60 LEN 1 TIM 6
	system->cpu.opcode[0x60] = { inst_RTS, 1, 6 };
	// DEX  (DEcrement X) 0xCA LEN 1 TIM 2
	system->cpu.opcode[0xCA] = { inst_DEX, 1, 2 };
	// DEY  (DEcrement Y) 0x88 LEN 1 TIM 2
	system->cpu.opcode[0x88] = { inst_DEY, 1, 2 };
	// BPL  (Branch On Equal) 0x10 LEN 2 TIM 2
	system->cpu.opcode[0x10] = { inst_BPL, 2, 2 };
	// INX  (INcrement X) 0xE8 LEN 1 TIM 2
	system->cpu.opcode[0xE8] = { inst_INX, 1, 2 };
	// INC  (Zero Page) 0xE6 LEN 2 TIM 5
	system->cpu.opcode[0xE6] = { inst_INC_Z, 2, 5 };
	// STA  (Absolute,X) 0x9D LEN 3 TIM 5
	system->cpu.opcode[0x9D] = { inst_STA_AX, 3, 5 };
	// STA  (Absolute,Y) 0x99 LEN 3 TIM 5
	system->cpu.opcode[0x99] = { inst_STA_AY, 3, 5 };
	// STA  (Zero Page, X) 0x95 LEN 2 TIM 4
	system->cpu.opcode[0x95] = { inst_STA_ZX, 2, 4 };
	// LDA (Zero Page, X) 0xB5 LEN 2 TIM 4
	system->cpu.opcode[0xB5] = { inst_LDA_ZX, 2, 4 };
	// ASL  (Arithmetic Shift Left) 0x0A LEN 1 TIM 2
	system->cpu.opcode[0x0A] = { inst_ASL, 1, 2 };
	// TAY  (Transfer A to Y) 0xA8 LEN 1 TIM 2
	system->cpu.opcode[0xA8] = { inst_TAY, 1, 2 };
	// SBC (Zerropage) 0xE5 LEN 2 TIM 3
	system->cpu.opcode[0xE5] = { inst_SBC_Z, 2, 3 };
	// SBC (Immediate) 0xE9 LEN 2 TIM 2
	system->cpu.opcode[0xE9] = { inst_SBC_I, 2, 2 };
	// SEC (SEt Carry) 0x38 LEN 1 TIM 2
	system->cpu.opcode[0x38] = { inst_SEC_IM, 1, 2 };
	// LSR (Logical Shift Right) 0x4A LEN 1 TIM 2
	system->cpu.opcode[0x4A] = { inst_LSR, 1, 2 };
	// AND (Zerro Page) 0x25 LEN 2 TIM 3
	system->cpu.opcode[0x25] = { inst_AND_Z, 2, 3 };
	// STA  (Absolute) 0x8D LEN 3 TIM 4
	system->cpu.opcode[0x8D] = { inst_STA_A, 3, 4 };
	// EOR (Immediate) 0x49 LEN 2 TIM 2
	system->cpu.opcode[0x49] = { inst_EOR_I, 2, 2 };
	// INC (Absolute,X) 0xfe LEN 2 TIM 2
	system->cpu.opcode[0xfe] = { inst_INC_AX, 3, 7 };
	// ROR (Zero Page) 0x66 LEN 2 TIM 5
	system->cpu.opcode[0x66] = { inst_ROR_Z, 2, 5 };
	// ROL (Accumulator) 0x2A LEN 1 TIM 2
	system->cpu.opcode[0x2A] = { inst_ROL, 1, 2 };


	// NOP (Implied) 0xea LEN 1 TIM 2
	system->cpu.opcode[0xea] = { inst_NOP, 1, 2 };

	// LDA  (Absolute) 0xAD LEN 3 TIM 4
	system->cpu.opcode[0xAD] = { inst_LDA_A, 3, 4 };



	// CMP  (Indirect,Y) 0xD1 LEN 2 TIM 5
	system->cpu.opcode[0xD1] = { inst_CMP_IY, 2, 5 };
	// PHA  (PusH Accumulator) 0x48 LEN 1 TIM 3
	system->cpu.opcode[0x48] = { inst_PHA, 1, 3 };
	// PLA  (PuLL Accumulator) 0x68 LEN 1 TIM 4
	system->cpu.opcode[0x68] = { inst_PLA, 1, 4 };
	// SBC (Zero Page, X) 0xf5 LEN 2 TIM 4
	system->cpu.opcode[0xf5] = { inst_SBC_ZX, 2, 4 };

	FILE* pRomFile;
	errno_t err;

	err = fopen_s(&pRomFile, "D:\\Programmierung\\C64 - SID\\asm\\tiny.o", "rb");
	
	if (err == 0)
		printf("The file 'crt_fopen_s.c' was opened\n");

	fseek(pRomFile, 0, SEEK_END);
	int size = ftell(pRomFile);
	fseek(pRomFile, 0, SEEK_SET);

	fread(&system->bus.mem[rom_start], size, 1, pRomFile);
	fclose(pRomFile);

#if 0
	/* ROM */
	system->bus.mem[rom_start++] = 0xA5;
	system->bus.mem[rom_start++] = 0xFE;
	system->bus.mem[rom_start++] = 0x85;
	system->bus.mem[rom_start++] = 0x00;
	system->bus.mem[rom_start++] = 0xA5;
	system->bus.mem[rom_start++] = 0xFE;
	system->bus.mem[rom_start++] = 0x29;
	system->bus.mem[rom_start++] = 0x03;
	system->bus.mem[rom_start++] = 0x18;
	system->bus.mem[rom_start++] = 0x69;
	system->bus.mem[rom_start++] = 0x02;
	system->bus.mem[rom_start++] = 0x85;
	system->bus.mem[rom_start++] = 0x01;
	system->bus.mem[rom_start++] = 0xA5;
	system->bus.mem[rom_start++] = 0xFE;
	system->bus.mem[rom_start++] = 0xA0;
	system->bus.mem[rom_start++] = 0x00;
	system->bus.mem[rom_start++] = 0x91;
	system->bus.mem[rom_start++] = 0x00;
	system->bus.mem[rom_start++] = 0x4C;
	system->bus.mem[rom_start++] = 0x00;
	system->bus.mem[rom_start++] = 0x80;
#endif

	system->cpu.reg.sp = 0xff;
	system->cpu.reg.pc.value = 0x600;
	time_t t;
	srand((unsigned int) time(&t));

#if 0
	while(1)
	{
		system->bus.mem[0xfe] = rand() & 0xff;
		opByte = system->bus.mem[system->cpu.reg.pc.value];
		system->cpu.reg.pc.value = system->cpu.opcode[opByte].fkt(system, &system->bus.mem[system->cpu.reg.pc.value + 1], 
																  system->cpu.opcode[opByte].len, system->cpu.opcode[opByte].cyles);
	}
#endif
}

void tick_6502_system(struct system* system)
{
	uint8_t  opByte;
	system->bus.mem[0xfe] = rand() & 0xff;
	opByte = system->bus.mem[system->cpu.reg.pc.value];

	system->cpu.reg.pc.value = system->cpu.opcode[opByte].fkt(system, &system->bus.mem[system->cpu.reg.pc.value + 1],
															  system->cpu.opcode[opByte].len, system->cpu.opcode[opByte].cyles);
}

uint16_t inst_unknown(struct system *system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	int test = 0;
	return system->cpu.reg.pc.value;
}

uint16_t inst_LDA_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = opPara[0];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDA_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t zpAdr;
	uint16_t addr;
	zpAdr = (system->cpu.reg.x + opPara[0]);
	addr = (system->bus.mem[zpAdr + 1] << 8) + (system->bus.mem[zpAdr]);
	system->cpu.reg.a = system->bus.mem[addr];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDA_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0] + 0]);
	addr += system->cpu.reg.y;
	system->cpu.reg.a = system->bus.mem[addr];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CMP_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
#if 0
	uint16_t addr;
	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0] + 0]);
	addr += system->cpu.reg.y;
	system->cpu.reg.a = system->bus.mem[addr];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
#endif

	uint8_t cmp;

	uint16_t addr;
	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0] + 0]);
	addr += system->cpu.reg.y;

	cmp = system->cpu.reg.a - system->bus.mem[addr];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= system->bus.mem[addr]);
	system->cpu.reg.flags.negative = (cmp & 0x80) != 0;
	system->cpu.reg.flags.zero = (cmp == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDX_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.x = opPara[0];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.x & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.x == 0);
	return system->cpu.reg.pc.value + len;
}

// OK ?
uint16_t inst_LDA_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->bus.mem[opPara[0]];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDX_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.x = system->bus.mem[opPara[0]];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.x & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.x == 0);
	return system->cpu.reg.pc.value + len;
}

// OK?
uint16_t inst_STA_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->bus.mem[opPara[0]] = system->cpu.reg.a;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STX_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->bus.mem[opPara[0]] = system->cpu.reg.x;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STY_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->bus.mem[opPara[0]] = system->cpu.reg.y;
	return system->cpu.reg.pc.value + len;
}

// OK?
uint16_t inst_AND_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->cpu.reg.a & opPara[0];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_AND_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->cpu.reg.a & system->bus.mem[opPara[0]];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}
// OK?
uint16_t inst_CLC_IM(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.flags.carry = 0;
	// affects no other Flags
	return system->cpu.reg.pc.value + len;
}

// OK?
uint16_t inst_SEC_IM(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.flags.carry = 1;
	// affects no other Flags
	return system->cpu.reg.pc.value + len;
}

// OK?
uint16_t inst_ADC_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t add;
	add = system->cpu.reg.a + opPara[0] + system->cpu.reg.flags.carry;
	system->cpu.reg.a = add;
	// affects C,N,Z, V Flags
	system->cpu.reg.flags.carry = (add & 0x100) != 0;
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	//system->cpu.reg.flags.overflow = TODO
	return system->cpu.reg.pc.value + len;
}

// OK?
uint16_t inst_ADC_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t add;
	add = system->cpu.reg.a + system->bus.mem[opPara[0]] + system->cpu.reg.flags.carry;
	system->cpu.reg.a = add;
	// affects C,N,Z, V Flags
	system->cpu.reg.flags.carry = (add & 0x100) != 0;
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	//system->cpu.reg.flags.overflow = TODO
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ADC_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	uint16_t add;
	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.x;
	add = system->cpu.reg.a + system->bus.mem[addr] + system->cpu.reg.flags.carry;
	system->cpu.reg.a = add;
	// affects C,N,Z, V Flags
	system->cpu.reg.flags.carry = (add & 0x100) != 0;
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	//system->cpu.reg.flags.overflow = TODO
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ADC_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	uint16_t add;
	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0] + 0]);
	addr += system->cpu.reg.y;
	add = system->cpu.reg.a + system->bus.mem[addr] + system->cpu.reg.flags.carry;
	system->cpu.reg.a = add;
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_SBC_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t add;
	add = system->cpu.reg.a + (~system->bus.mem[opPara[0]]) + system->cpu.reg.flags.carry;
	system->cpu.reg.a = (uint8_t) add;
	// affects C,N,Z, V Flags
	system->cpu.reg.flags.carry = (add & 0x100) != 0;
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	//system->cpu.reg.flags.overflow = TODO
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_SBC_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t add;
	add = system->cpu.reg.a + ~opPara[0] + system->cpu.reg.flags.carry;
	system->cpu.reg.a = (uint8_t)add;
	// affects C,N,Z, V Flags
	system->cpu.reg.flags.carry = (add & 0x100) != 0;
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	//system->cpu.reg.flags.overflow = TODO
	return system->cpu.reg.pc.value + len;
}

// OK?
uint16_t inst_LDY_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.y = opPara[0];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.y & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.y == 0);
	return system->cpu.reg.pc.value + len;
}

// OK?
uint16_t inst_STA_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0] + 0]);
	addr += system->cpu.reg.y;
	system->bus.mem[addr] = system->cpu.reg.a;
	// affects no Flags
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_JMP_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word jmpAdr;
	jmpAdr.s.lo = opPara[0];
	jmpAdr.s.hi = opPara[1];
	// affects no Flags
	return jmpAdr.value;
}

uint16_t inst_STA_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->bus.mem[adr.value] = system->cpu.reg.a;
	// affects no Flags
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDA_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.a = system->bus.mem[adr.value];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STA_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t zpAdr;
	uint16_t addr;
	zpAdr = (system->cpu.reg.x + opPara[0]);
	addr = (system->bus.mem[zpAdr + 1] << 8) + (system->bus.mem[zpAdr]);
	system->bus.mem[addr] = system->cpu.reg.a;
	// affects no Flags
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_INC_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t newVal;
	system->bus.mem[opPara[0]]++;
	newVal = system->bus.mem[opPara[0]];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (newVal & 0x80) != 0;
	system->cpu.reg.flags.zero = (newVal == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_DEC_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t newVal;
	system->bus.mem[opPara[0]]--;
	newVal = system->bus.mem[opPara[0]];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (newVal & 0x80) != 0;
	system->cpu.reg.flags.zero = (newVal == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CMP_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cmp;
	cmp = system->cpu.reg.a - opPara[0];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= opPara[0]);
	system->cpu.reg.flags.negative = (cmp & 0x80) != 0;
	system->cpu.reg.flags.zero = (cmp == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CMP_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cmp;
	cmp = system->cpu.reg.a - system->bus.mem[opPara[0]];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= system->bus.mem[opPara[0]]);
	system->cpu.reg.flags.negative = (cmp & 0x80) != 0;
	system->cpu.reg.flags.zero = (cmp == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CMY_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cmp;
	cmp = system->cpu.reg.y - opPara[0];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.y >= opPara[0]);
	system->cpu.reg.flags.negative = (cmp & 0x80) != 0;
	system->cpu.reg.flags.zero = (cmp == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CMX_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cmp;
	cmp = system->cpu.reg.x - opPara[0];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.x >= opPara[0]);
	system->cpu.reg.flags.negative = (cmp & 0x80) != 0;
	system->cpu.reg.flags.zero = (cmp == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_BNE(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t newPc;
	newPc  = system->cpu.reg.pc.value + len;
	if(!system->cpu.reg.flags.zero)
		newPc += (int8_t)opPara[0];
	// affects no Flags
	return newPc;
}

uint16_t inst_BCC(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t newPc;
	newPc = system->cpu.reg.pc.value + len;
	if (!system->cpu.reg.flags.carry)
		newPc += (int8_t)opPara[0];
	// affects no Flags
	return newPc;
}

uint16_t inst_BEQ(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t newPc;
	newPc = system->cpu.reg.pc.value + len;
	if (system->cpu.reg.flags.zero)
		newPc += (int8_t)opPara[0];
	// affects no Flags
	return newPc;
}

uint16_t inst_TAX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.x = system->cpu.reg.a;
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.x & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.x == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_TXA(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->cpu.reg.x;
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.x & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.x == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_TYA(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->cpu.reg.y;
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.y & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.y == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_INY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.y++;
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.y & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.y == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_JSR(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t pc;
	pc = system->cpu.reg.pc.value + len;
	pc = pc - 1;
	// auf dem Stack schreiben
	system->bus.mem[0x100 + system->cpu.reg.sp] = (pc & 0xff00) >> 8;
	system->cpu.reg.sp--;
	system->bus.mem[0x100 + system->cpu.reg.sp] = (pc & 0xff);
	system->cpu.reg.sp--;
	pc = (opPara[1] << 8) + opPara[0];
	// affects no Flags
	return pc;
}

uint16_t inst_LDY_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.y = system->bus.mem[opPara[0]];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.y & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.y == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_RTS(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t pc;
	uint8_t  lo;
	uint8_t  hi;
	// vom Stack holen
	system->cpu.reg.sp++;
	lo = system->bus.mem[0x100 + system->cpu.reg.sp];
	system->cpu.reg.sp++;
	hi = system->bus.mem[0x100 + system->cpu.reg.sp];
	pc = (hi << 8);
	pc += lo + 1;
	// affects no Flags
	return pc;
}

uint16_t inst_DEX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.x--;
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.x & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.x == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_DEY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.y--;
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.y & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.y == 0);
	return system->cpu.reg.pc.value + len;
}

// NEU
uint16_t inst_LDA_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.x;
	system->cpu.reg.a = system->bus.mem[addr];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_INC_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.x;
	system->bus.mem[addr]++;
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->bus.mem[addr]) != 0;
	system->cpu.reg.flags.zero = (system->bus.mem[addr] == 0);
	return system->cpu.reg.pc.value + len;
}

// NEU
uint16_t inst_LDA_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.y;
	system->cpu.reg.a = system->bus.mem[addr];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_BPL(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t newPc;
	newPc = system->cpu.reg.pc.value + len;
	
	if (!system->cpu.reg.flags.negative)
		newPc += (int8_t)opPara[0];
	// affects no Flags
	return newPc;
}

uint16_t inst_INX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.x++;
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.x & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.x == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STA_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.x;
	system->bus.mem[addr] = system->cpu.reg.a;
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STA_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.y;
	system->bus.mem[addr] = system->cpu.reg.a;
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDA_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t addr;
	addr = (opPara[0] + system->cpu.reg.x);
	system->cpu.reg.a = system->bus.mem[addr];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_SBC_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t addr, add;
	addr = (opPara[0] + system->cpu.reg.x);
	add = system->cpu.reg.a + ~system->bus.mem[addr] + system->cpu.reg.flags.carry;
	system->cpu.reg.a = (uint8_t)add;
	// affects C,N,Z, V Flags
	system->cpu.reg.flags.carry = (add & 0x100) != 0;
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	//system->cpu.reg.flags.overflow = TODO
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STA_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t addr;
	addr = (opPara[0] + system->cpu.reg.x);
	system->bus.mem[addr] = system->cpu.reg.a;
	// affects no Flags
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ASL(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t shiftVal;
	shiftVal = (system->cpu.reg.a << 1);
	system->cpu.reg.a <<= 1;
	// affects C,N,Z
	system->cpu.reg.flags.carry = (shiftVal & 0x100) != 0;
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LSR(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t flag_carry;
	flag_carry = system->cpu.reg.a & 0x01;
	system->cpu.reg.a >>= 1;
	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_carry;
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ROR_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t flag_newCarry;
	flag_newCarry = system->bus.mem[opPara[0]] & 0x01;
	system->bus.mem[opPara[0]] >>= 1;
	system->bus.mem[opPara[0]] |= (system->cpu.reg.flags.carry << 7);
	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_newCarry;
	system->cpu.reg.flags.negative = (system->bus.mem[opPara[0]] & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->bus.mem[opPara[0]] == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ROL(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t flag_newCarry;
	flag_newCarry = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.a <<= 1;
	system->cpu.reg.a |= system->cpu.reg.flags.carry;
	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_newCarry;
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_NOP(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	// affects no Flags
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_PHA(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	// auf dem Stack schreiben
	system->bus.mem[0x100 + system->cpu.reg.sp] = system->cpu.reg.a;
	system->cpu.reg.sp--;
	// affects no Flags
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_PLA(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.sp++;
	// auf dem Stack schreiben
	system->cpu.reg.a = system->bus.mem[0x100 + system->cpu.reg.sp];
	// affects no Flags
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_TAY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.y = system->cpu.reg.a;
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.y & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.y == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_BMI(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t newPc;
	newPc = system->cpu.reg.pc.value + len;

	if (system->cpu.reg.flags.negative)
		newPc += (int8_t)opPara[0];
	// affects no Flags
	return newPc;
}

uint16_t inst_ORA_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->cpu.reg.a | system->bus.mem[opPara[0]];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_EOR_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->cpu.reg.a ^ opPara[0];
	// affects N,Z Flags
	system->cpu.reg.flags.negative = (system->cpu.reg.a & 0x80) != 0;
	system->cpu.reg.flags.zero = (system->cpu.reg.a == 0);
	return system->cpu.reg.pc.value + len;
}


