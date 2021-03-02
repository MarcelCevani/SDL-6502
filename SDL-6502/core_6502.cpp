#include "core_6502.h"

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <memory.h>
#include <time.h>

#include <winsock2.h>
#include <ws2tcpip.h>

extern SOCKET s;
extern void write_emu_u8(SOCKET s, uint16_t adr, uint8_t value);

inline void update_NZ_Flags(struct system* system, uint8_t value);
inline void update_V_Flag(struct system* system, uint8_t add_op1, uint8_t add_op2, uint8_t add_result);

void memory_write(struct system* system, uint16_t addr, uint8_t value);

uint16_t inst_LDA_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDA_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ORA_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDA_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDA_Z(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDA_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STA_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDA_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDY_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_INC_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDA_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STA_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STA_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDX_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDX_ZY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDY_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDX_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDX_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDY_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDY_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDX_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STA_Z(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STX_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STX_ZY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STX_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STY_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STY_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STY_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STA_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STA_IY(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);
uint16_t inst_AND_I(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);
uint16_t inst_AND_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_AND_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_AND_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_AND_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_AND_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CLC_IM(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);
uint16_t inst_SEC_IM(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ADC_I(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);

uint16_t inst_ADC_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ADC_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_ADC_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ADC_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ADC_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_SBC_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_SBC_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_SBC_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_SBC_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_SBC_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_ADC_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ADC_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_LDY_I(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);
uint16_t inst_JMP_A(struct system *system, uint8_t *opPara, uint32_t len, uint32_t cyles);
uint16_t inst_JMP_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_STA_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LDA_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_INC_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_INC_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_INC_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_DEC_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_DEC_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_DEC_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_DEC_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CMP_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CMP_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CMP_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CPY_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CMP_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CMP_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CMP_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CMP_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_CPY_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CMX_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CPX_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_CMP_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CPX_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CPY_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CPY_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_BNE(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_BCC(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_BCS(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_BPL(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_BMI(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_BEQ(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_BVC(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_BVS(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
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
uint16_t inst_ASL_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ASL_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ASL_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ASL_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LSR(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LSR_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LSR_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_LSR_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_LSR_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_ROL(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ROL_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ROL_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_ROL_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ROL_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ROR_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ROR_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ROR_REG(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ROR_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ROR_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_SBC_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_SBC_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_SBC_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ORA_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ORA_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_ORA_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ORA_IM(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ORA_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ORA_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_ORA_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_EOR_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_EOR_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_EOR_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_EOR_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_EOR_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_EOR_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_EOR_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);

uint16_t inst_EOR_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_AND_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_AND_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_NOP(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_PHA(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_PLA(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_PLP(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_PHP(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_ADC_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_BIT_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_BIT_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_SEI(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CLI(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CLV(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_SED(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_CLD(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_TXS(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_TSX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_INC_SBC_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_RTI(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles);
uint16_t inst_unknown(struct system* system, uint8_t *opPara, uint32_t len, uint32_t cyles);

void init_6502_sytem(struct system *system)
{
	uint32_t i;
	uint32_t fkt_count;
	//uint32_t rom_start = 0xA5FE;
	
	//uint32_t rom_start = 0xA5FE; // Paperboy


	uint32_t rom_start = 0x0FF6; // Celestrial



	//uint32_t rom_start = 0x224;

	//uint8_t  opByte;

	memset(system, 0x00, sizeof(struct system));

	for (i = 0; i < 256; i++)
		system->cpu.opcode[i].fkt = inst_unknown;

	// ORA (Zero Page) 0x05 LEN 2 TIM 3
	system->cpu.opcode[0x05] = { inst_ORA_Z, 2, 3 , "ORA" };  // x
	// ORA (Zero Page,X) 0x15 LEN 2 TIM 4
	system->cpu.opcode[0x15] = { inst_ORA_ZX, 2, 4 , "ORA" };  // x
	// ORA (Absolute) 0x0d LEN 3 TIM 4
	system->cpu.opcode[0x0d] = { inst_ORA_A, 3, 4 , "ORA" };  // x
    // ORA (Implied) 0x09 LEN 2 TIM 2
	system->cpu.opcode[0x09] = { inst_ORA_IM, 2, 2 , "ORA" };  // x
	// ORA (Absolute,X) 0x1d LEN 3 TIM 4
	system->cpu.opcode[0x1d] = { inst_ORA_AX, 3, 4 , "ORA" };  // x
	// ORA (Absolute,Y) 0x19 LEN 3 TIM 4
	system->cpu.opcode[0x19] = { inst_ORA_AY, 3, 4 , "ORA" };  // x
	// LDY  (Immidiate) 0xa0 LEN 2 TIM 2	
	system->cpu.opcode[0xA0] = { inst_LDY_I, 2, 2 , "LDY" };  // x
	// CLC  (Implied)   0x18 LEN 1 TIM 2			  
	system->cpu.opcode[0x18] = { inst_CLC_IM, 1, 2, "CLC" }; // x
	// AND  (Immidiate) 0x29 LEN 2 TIM 2
	system->cpu.opcode[0x29] = { inst_AND_I, 2, 2 , "AND" };  // x
	// AND  (Indirect, X) 0x21 LEN 2 TIM 6
	system->cpu.opcode[0x21] = { inst_AND_IX, 2, 6 , "AND" };  // x
	// AND  (Indirect, Y) 0x31 LEN 2 TIM 5
	system->cpu.opcode[0x31] = { inst_AND_IY, 2, 5 , "AND" };  // x
	// AND  (Absolute) 0x2D LEN 3 TIM 4
	system->cpu.opcode[0x2d] = { inst_AND_A, 3, 4 , "AND" };  // x
	// AND  (Absolute,Y) 0x39 LEN 3 TIM 4
	system->cpu.opcode[0x39] = { inst_AND_AY, 3, 4 , "AND" };  // x
    // AND  (Absolute,X) 0x3D LEN 3 TIM 4
	system->cpu.opcode[0x3d] = { inst_AND_AX, 3, 4 , "AND" };  // x
	// JMP  (Absolute) 0x4C LEN 3 TIM 3
	system->cpu.opcode[0x4c] = { inst_JMP_A, 3, 3 , "JMP" };  // x
	// JMP  (Indirekt) 0x6C LEN 3 TIM 3
	system->cpu.opcode[0x6c] = { inst_JMP_I, 3, 5, "JMP" };  // x
	// ADC  (Immidiate) 0x69 LEN 2 TIM 3			  // Overflow TODO
	system->cpu.opcode[0x69] = { inst_ADC_I, 2, 2 , "ADC" };
	// ADC  (Indirekt, X) 0x61 LEN 2 TIM 6			  // Overflow TODO
	system->cpu.opcode[0x61] = { inst_ADC_IX, 2, 6 , "ADC" };
	// ADC  (Indirekt, Y) 0x71 LEN 2 TIM 6			  // Overflow TODO
	system->cpu.opcode[0x71] = { inst_ADC_IY, 2, 6 , "ADC" };
	// ADC  (Zerro Page) 0x65 LEN 2 TIM 3			  // Overflow TODO
	system->cpu.opcode[0x65] = { inst_ADC_Z, 2, 3 , "ADC" };

	// ADC  (Zerro Page,X) 0x75 LEN 2 TIM 4			  
	system->cpu.opcode[0x75] = { inst_ADC_ZX, 2, 4 , "ADC" };

	// ADC  (Absolute) 0x6D LEN 3 TIM 4			  // Overflow TODO
	system->cpu.opcode[0x6D] = { inst_ADC_A, 3, 4 , "ADC" };
	
	// ADC  (Absolute,X) 0x7d LEN 3 TIM 4			  // Overflow TODO
	system->cpu.opcode[0x7d] = { inst_ADC_AX, 3, 4 , "ADC" };
	
	// ADC  (Absolute,Y) 0x79 LEN 3 TIM 4			  // Overflow TODO
	system->cpu.opcode[0x79] = { inst_ADC_AY, 3, 4 , "ADC" };

	
	// ADC  (Indirect,Y) 0x71 LEN 2 TIM 5
	system->cpu.opcode[0x71] = { inst_ADC_IY, 2, 5 , "ADC" };

	// LDA  (Indirect,X) 0xa1 LEN 2 TIM 2
	system->cpu.opcode[0xa1] = { inst_LDA_IX, 2, 2 , "LDA" }; // ok?
	// LDX  (Immidiate) 0xa2 LEN 2 TIM 2
	system->cpu.opcode[0xa2] = { inst_LDX_I, 2, 2 , "LDX" }; // ok?
	// LDA  (Zero Page) 0xa5 LEN 2 TIM 3
	system->cpu.opcode[0xa5] = { inst_LDA_Z, 2, 3 , "LDA" }; // ok?
	// LDX  (Zero Page) 0xa6 LEN 2 TIM 3
	system->cpu.opcode[0xa6] = { inst_LDX_Z, 2, 3, "LDX" }; // ok?
	// LDX  (Zero Page, Y) 0xb6 LEN 2 TIM 3
	system->cpu.opcode[0xb6] = { inst_LDX_ZY, 2, 3, "LDX" }; // ok?
	
	// LDY  (Zero Page, X) 0xb4 LEN 2 TIM 3
	system->cpu.opcode[0xb4] = { inst_LDY_ZX, 2, 3, "LDY" }; // ok?

	// LDX  (Absolute) 0xae LEN 3 TIM 4
	system->cpu.opcode[0xae] = { inst_LDX_A, 3, 4 , "LDX" }; // ok?
	// LDX  (Absolute,Y) 0xbe LEN 3 TIM 4
	system->cpu.opcode[0xbe] = { inst_LDX_AY, 3, 4 , "LDX" }; // ok?
	// LDY  (Absolute) 0xac LEN 3 TIM 4
	system->cpu.opcode[0xac] = { inst_LDY_A, 3, 4 , "LDY" }; // ok?
	// DEC  (Zero Page) 0xC6 LEN 2 TIM 5
	system->cpu.opcode[0xC6] = { inst_DEC_Z, 2, 5 , "DEC" }; // ok?
	
	// DEC  (Zero Page,X) 0xD6 LEN 2 TIM 6
	system->cpu.opcode[0xD6] = { inst_DEC_ZX, 2, 6 , "DEC" }; // ok?


	// DEC  (Absolute) 0xCE LEN 3 TIM 6
	system->cpu.opcode[0xCE] = { inst_DEC_A, 3, 6 , "DEC" }; // ok?
	// DEC  (Absolute,X) 0xde LEN 3 TIM 7
	system->cpu.opcode[0xde] = { inst_DEC_AX, 3, 7, "DEC" }; // ok?
	// LDA  (Immediate) 0xa9 LEN 2 TIM 2
	system->cpu.opcode[0xa9] = { inst_LDA_I, 2, 2, "LDA" };
	// LDA  (Absolute,X) 0xbd LEN 3 TIM 4+
	system->cpu.opcode[0xbd] = { inst_LDA_AX, 3, 4, "LDA" };
	// LDA  (Absolute,Y) 0xb9 LEN 3 TIM 4+
	system->cpu.opcode[0xb9] = { inst_LDA_AY, 3, 4, "LDA" };
	// CMP  (Immediate) 0xc9 LEN 2 TIM 2
	system->cpu.opcode[0xc9] = { inst_CMP_I, 2, 2, "CMP" };
	
	// CMP  (Zero Page) 0xc5 LEN 2 TIM 2
	system->cpu.opcode[0xc5] = { inst_CMP_Z, 2, 3 , "CMP" };
	
	// CMP  (Absolute) 0xcd LEN 3 TIM 4
	system->cpu.opcode[0xcd] = { inst_CMP_A, 3, 4 , "CMP" };
	

	// CPY  (Absolute) 0xcc LEN 3 TIM 4
	system->cpu.opcode[0xcc] = { inst_CPY_A, 3, 4 , "CPY" };


	
	// CMP  (Absolute,X) 0xdd LEN 3 TIM 4
	system->cpu.opcode[0xdd] = { inst_CMP_AX, 3, 4 , "CMP" };
	
	// CMP  (Absolute,Y) 0xd9 LEN 3 TIM 4
	system->cpu.opcode[0xd9] = { inst_CMP_AY, 3, 4 , "CMP" };


	// CPY  (Immediate) 0xc0 LEN 2 TIM 2
	system->cpu.opcode[0xc0] = { inst_CPY_I, 2, 2 , "CPY" };


	// CPY  (Zerropage) 0xc4 LEN 2 TIM 3
	system->cpu.opcode[0xc4] = { inst_CPY_Z, 2, 3 , "CPY" };


	// CMX  (Immediate) 0xe0 LEN 2 TIM 2
	system->cpu.opcode[0xe0] = { inst_CMX_I, 2, 2 , "CMX" };
	// BNE  (Branch not Equal) 0xd0 LEN 2 TIM 2
	system->cpu.opcode[0xd0] = { inst_BNE, 2, 2 , "BNE" };
	// BMI  (Branch on MInus) 0x30 LEN 2 TIM 2
	system->cpu.opcode[0x30] = { inst_BMI, 2, 2 , "MBI" };
	// BEQ  (Branch On Equal) 0xf0 LEN 2 TIM 2
	system->cpu.opcode[0xf0] = { inst_BEQ, 2, 2, "BEQ" };
	// BCC  (Branch on Carry Clear) 0x90 LEN 2 TIM 2
	system->cpu.opcode[0x90] = { inst_BCC, 2, 2 , "BCC" };
	// BCS  (Branch on Carry Set) 0xB0 LEN 2 TIM 2
	system->cpu.opcode[0xB0] = { inst_BCS, 2, 2 , "BCS" };
	// STA  (Indirect, X) 0x81 LEN 2 TIM 6
	system->cpu.opcode[0x81] = { inst_STA_IX, 2, 6 , "STA" }; // ok?
	// STA  (Zero Page) 0x85 LEN 2 TIM 3
	system->cpu.opcode[0x85] = { inst_STA_Z, 2, 3 , "STA" }; // ok?
	// STA  (Immidiate Y) 0x91 LEN 2 TIM 6
	system->cpu.opcode[0x91] = { inst_STA_IY, 2, 6 , "STA" }; // ok?
	// LDA  (Indirect, Y) 0xB1 LEN 2 TIM 5
	system->cpu.opcode[0xB1] = { inst_LDA_IY, 2, 5 , "LDA" }; // ok?
	// TAX  (Transfer A to X) 0xAA LEN 1 TIM 2
	system->cpu.opcode[0xAA] = { inst_TAX, 1, 2 , "TAX" }; // ok?
	// TXA  (Transfer X to A) 0x8A LEN 1 TIM 2
	system->cpu.opcode[0x8A] = { inst_TXA, 1, 2 , "TXA" }; // ok?
	// TYA  (Transfer Y to A) 0x98 LEN 1 TIM 2
	system->cpu.opcode[0x98] = { inst_TYA, 1, 2, "TYA" }; // ok?
	// INY  (INcrement Y) 0xC8 LEN 1 TIM 2
	system->cpu.opcode[0xC8] = { inst_INY, 1, 2, "INY" }; // ok?
	// JSR  (Jump to SubRoutine) 0x20 LEN 3 TIM 2
	system->cpu.opcode[0x20] = { inst_JSR, 3, 6 ,"JSR" }; // ok?
	// STY  (Zero Page) 0x84 LEN 2 TIM 3
	system->cpu.opcode[0x84] = { inst_STY_Z, 2, 3 , "STY" };
	
	// STY  (Zero Page, X) 0x94 LEN 2 TIM 4
	system->cpu.opcode[0x94] = { inst_STY_ZX, 2, 4 , "STY" };


	// STY  (Absolute) 0x8c LEN 2 TIM 3
	system->cpu.opcode[0x8c] = { inst_STY_A, 3, 4, "STY" };
	// STX  (Zero Page) 0x86 LEN 2 TIM 3
	system->cpu.opcode[0x86] = { inst_STX_Z, 2, 3 , "STX" };

	// STX  (Zero Page,Y) 0x96 LEN 2 TIM 3
	system->cpu.opcode[0x96] = { inst_STX_ZY, 2, 4 , "STX" };


	// STX  (Absolute) 0x8E LEN 3 TIM 4
	system->cpu.opcode[0x8E] = { inst_STX_A, 3, 4 , "STX" };
	// LDY  (Zero Page) 0xA4 LEN 2 TIM 3
	system->cpu.opcode[0xA4] = { inst_LDY_Z, 2, 3 , "LDY" };
	// RTS  (ReTurn from Subroutine) 0x60 LEN 1 TIM 6
	system->cpu.opcode[0x60] = { inst_RTS, 1, 6 , "RTS" };
	// DEX  (DEcrement X) 0xCA LEN 1 TIM 2
	system->cpu.opcode[0xCA] = { inst_DEX, 1, 2 , "DEX" };
	// DEY  (DEcrement Y) 0x88 LEN 1 TIM 2
	system->cpu.opcode[0x88] = { inst_DEY, 1, 2 , "DEY" };
	// BPL  (Branch On Equal) 0x10 LEN 2 TIM 2
	system->cpu.opcode[0x10] = { inst_BPL, 2, 2 , "BPL" };
	// INX  (INcrement X) 0xE8 LEN 1 TIM 2
	system->cpu.opcode[0xE8] = { inst_INX, 1, 2 , "INX" };
	// INC  (Zero Page) 0xE6 LEN 2 TIM 5
	system->cpu.opcode[0xE6] = { inst_INC_Z, 2, 5 , "INC" };
	
	// INC  (Zero Page,X) 0xF6 LEN 2 TIM 6
	system->cpu.opcode[0xF6] = { inst_INC_ZX, 2, 6 , "INC" };

	// INC  (Absolute) 0xEE LEN 3 TIM 6
	system->cpu.opcode[0xEE] = { inst_INC_A, 3, 6 , "INC"};
	// STA  (Absolute,X) 0x9D LEN 3 TIM 5
	system->cpu.opcode[0x9D] = { inst_STA_AX, 3, 5 , "STA" };
	// STA  (Absolute,Y) 0x99 LEN 3 TIM 5
	system->cpu.opcode[0x99] = { inst_STA_AY, 3, 5 , "STA" };
	// STA  (Zero Page, X) 0x95 LEN 2 TIM 4
	system->cpu.opcode[0x95] = { inst_STA_ZX, 2, 4 , "STA" };
	// LDA (Zero Page, X) 0xB5 LEN 2 TIM 4
	system->cpu.opcode[0xB5] = { inst_LDA_ZX, 2, 4 , "LDA" };
	// ASL  (Arithmetic Shift Left) 0x0A LEN 1 TIM 2
	system->cpu.opcode[0x0A] = { inst_ASL, 1, 2 , "ASL" };
	
	// ASL  (Absolute) 0x0E LEN 1 TIM 2
	system->cpu.opcode[0x0E] = { inst_ASL_A, 3, 6 , "ASL" };

	// ASL  (Absolute,X) 0x1E LEN 3 TIM 7
	system->cpu.opcode[0x1E] = { inst_ASL_AX, 3, 7 , "ASL" };


	// ASL  (Arithmetic Shift Left, ZerroPage) 0x06 LEN 2 TIM 5
	system->cpu.opcode[0x06] = { inst_ASL_Z, 2, 5 , "ASL" };

	// ASL  (ZerroPage,X) 0x16 LEN 2 TIM 5
	system->cpu.opcode[0x16] = { inst_ASL_ZX, 2, 6 , "ASL" };

	
	// TAY  (Transfer A to Y) 0xA8 LEN 1 TIM 2
	system->cpu.opcode[0xA8] = { inst_TAY, 1, 2 , "TAY" };
	
	// SBC (Zerropage) 0xE5 LEN 2 TIM 3
	system->cpu.opcode[0xE5] = { inst_SBC_Z, 2, 3 , "SBC" };
	
	// SBC (Immediate) 0xE9 LEN 2 TIM 2
	system->cpu.opcode[0xE9] = { inst_SBC_I, 2, 2 , "SBC" };
	
	
	
	// SBC (Indirekt,X) 0xE1 LEN 2 TIM 6
	system->cpu.opcode[0xE1] = { inst_SBC_IX, 2, 6 , "SBC" };

	// SBC (Indirekt,Y) 0xF1 LEN 2 TIM 5
	system->cpu.opcode[0xF1] = { inst_SBC_IY, 2, 5 , "SBC" };


	// SBC (Absolute) 0xED LEN 3 TIM 4
	system->cpu.opcode[0xED] = { inst_SBC_A, 3, 4 , "SBC" };

	
	// SBC (Absolute,X) 0xFD LEN 3 TIM 4
	system->cpu.opcode[0xFD] = { inst_SBC_AX, 3, 4 , "SBC" };

	// SBC (Absolute,Y) 0xF9 LEN 3 TIM 4
	system->cpu.opcode[0xF9] = { inst_SBC_AY, 3, 4 , "SBC" };



	// SEC (SEt Carry) 0x38 LEN 1 TIM 2
	system->cpu.opcode[0x38] = { inst_SEC_IM, 1, 2 , "SEC" };
	// LSR (Logical Shift Right) 0x4A LEN 1 TIM 2
	system->cpu.opcode[0x4A] = { inst_LSR, 1, 2 , "LSR" };
	// LSR (Logical Shift Right, ZerroPage) 0x46 LEN 2 TIM 5
	system->cpu.opcode[0x46] = { inst_LSR_Z, 2, 5 , "LSR" };
	
	
	
	// LSR (ZerroPage,X) 0x56 LEN 2 TIM 5
	system->cpu.opcode[0x56] = { inst_LSR_ZX, 2, 6 , "LSR" };

	
	
	// LSR (Logical Shift Right) 0x4A LEN 1 TIM 2
	system->cpu.opcode[0x4E] = { inst_LSR_A, 3, 6 , "LSR" };
	
	// LSR (Absolute,X) 0x5e LEN 3 TIM 7
	system->cpu.opcode[0x5e] = { inst_LSR_AX, 3, 7 , "LSR" };

	
	// AND (Zerro Page) 0x25 LEN 2 TIM 3
	system->cpu.opcode[0x25] = { inst_AND_Z, 2, 3 , "AND" };

	// AND (Zerro Page,X) 0x35 LEN 2 TIM 4
	system->cpu.opcode[0x35] = { inst_AND_ZX, 2, 4 , "AND" };


	// STA  (Absolute) 0x8D LEN 3 TIM 4
	system->cpu.opcode[0x8D] = { inst_STA_A, 3, 4 , "STA" };
	
	// EOR (Immediate) 0x49 LEN 2 TIM 2
	system->cpu.opcode[0x49] = { inst_EOR_I, 2, 2 , "EOR" };
	
	// EOR (Zeropage) 0x45 LEN 2 TIM 3
	system->cpu.opcode[0x45] = { inst_EOR_Z, 2, 3 , "EOR" };


	// EOR (Indirekt, X) 0x41 LEN 2 TIM 6
	system->cpu.opcode[0x41] = { inst_EOR_IX, 2, 6 , "EOR" };

	// EOR (Zeropage,X) 0x55 LEN 2 TIM 4
	system->cpu.opcode[0x55] = { inst_EOR_ZX, 2, 4 , "EOR" };


	// EOR (Absolute) 0x4D LEN 3 TIM 4
	system->cpu.opcode[0x4D] = { inst_EOR_A, 3, 4 , "EOR" };

	// EOR (Absolute,X) 0x5D LEN 3 TIM 4
	system->cpu.opcode[0x5D] = { inst_EOR_AX, 3, 4 , "EOR" };

	// EOR (Absolute,Y) 0x59 LEN 3 TIM 4
	system->cpu.opcode[0x59] = { inst_EOR_AY, 3, 4 , "EOR" };

	// EOR (Indirect,Y) 0x51 LEN 2 TIM 2
	system->cpu.opcode[0x51] = { inst_EOR_IY, 2, 2 , "EOR" };

	// INC (Absolute,X) 0xfe LEN 2 TIM 2
	system->cpu.opcode[0xfe] = { inst_INC_AX, 3, 7, "INC" };
	// ROR (Zero Page) 0x66 LEN 2 TIM 5
	system->cpu.opcode[0x66] = { inst_ROR_Z, 2, 5, "ROR" };


	// ROR (Zero Page,X) 0x76 LEN 2 TIM 6
	system->cpu.opcode[0x76] = { inst_ROR_ZX, 2, 6, "ROR" };

	// ROR (Register) 0x6a LEN 1 TIM 5
	system->cpu.opcode[0x6a] = { inst_ROR_REG, 1, 2, "ROR" };

	// ROR (Absolute) 0x6e LEN 3 TIM 6
	system->cpu.opcode[0x6e] = { inst_ROR_A, 3, 6, "ROR" };

	// ROR (Absolute,X) 0x7e LEN 3 TIM 7
	system->cpu.opcode[0x7e] = { inst_ROR_AX, 3, 7, "ROR" };

	// ROL (Accumulator) 0x2A LEN 1 TIM 2
	system->cpu.opcode[0x2A] = { inst_ROL, 1, 2, "ROL" };
	// ROL (ZerroPage) 0x26 LEN 2 TIM 5
	system->cpu.opcode[0x26] = { inst_ROL_Z, 2, 5, "ROL" };
	

	// ROL (ZerroPage,X) 0x36 LEN 2 TIM 6
	system->cpu.opcode[0x36] = { inst_ROL_ZX, 2, 6, "ROL" };


	// ROL (Absolute) 0x2e LEN 3 TIM 6
	system->cpu.opcode[0x2e] = { inst_ROL_A, 3, 6, "ROL" };
	
	// ROL (Absolute,X) 0x3e LEN 3 TIM 7
	system->cpu.opcode[0x3e] = { inst_ROL_AX, 3, 7, "ROL" };

	// NOP (Implied) 0xea LEN 1 TIM 2
	system->cpu.opcode[0xea] = { inst_NOP, 1, 2 , "NOP" };
	// LDA  (Absolute) 0xAD LEN 3 TIM 4
	system->cpu.opcode[0xAD] = { inst_LDA_A, 3, 4, "LDA" };
	// CMP  (Indirect,Y) 0xD1 LEN 2 TIM 5
	system->cpu.opcode[0xD1] = { inst_CMP_IY, 2, 5 , "CMP" };
	
	
	// CMP  (Indirect,X) 0xc1 LEN 2 TIM 5
	system->cpu.opcode[0xc1] = { inst_CMP_IX, 2, 6 , "CMP" };

	
	// PHA  (PusH Accumulator) 0x48 LEN 1 TIM 3
	system->cpu.opcode[0x48] = { inst_PHA, 1, 3 , "PHA" };
	// PLA  (PuLL Accumulator) 0x68 LEN 1 TIM 4
	system->cpu.opcode[0x68] = { inst_PLA, 1, 4 , "PLA" };
	// PLP  (PuLL Processor status) 0x68 LEN 1 TIM 4
	system->cpu.opcode[0x28] = { inst_PLP, 1, 4 , "PLP" };
	// PHP  (PusH Processor status) 0x08 LEN 1 TIM 3
	system->cpu.opcode[0x08] = { inst_PHP, 1, 3 , "PHP" };
	// SBC (Zero Page, X) 0xf5 LEN 2 TIM 4
	system->cpu.opcode[0xf5] = { inst_SBC_ZX, 2, 4, "SBC" };
	// LDY (Absolute,X) 0xbc 3 4+
	system->cpu.opcode[0xbc] = { inst_LDY_AX, 3, 4, "LDY" };
	// ORA (Indirect,X) 0x01 2 6
	system->cpu.opcode[0x01] = { inst_ORA_IX, 2, 6, "ORA" };
	// ORA (Indirect,Y) 0x11 2 5
	system->cpu.opcode[0x11] = { inst_ORA_IY, 2, 5, "ORA" };
	// BVC (Branch on oVerflow Clear) 0x50 2 2
	system->cpu.opcode[0x50] = { inst_BVC, 2, 2 , "BVC" };
	// BVS (Branch on oVerflow Set) 0x70 2 2
	system->cpu.opcode[0x70] = { inst_BVS, 2, 2 , "BVS" };
	// CPX (Zero Page) $E4  2   3
	system->cpu.opcode[0xe4] = { inst_CPX_Z, 2, 3 , "CPX" };
	// CMP (Zero Page,X) $d5  2   4
	system->cpu.opcode[0xd5] = { inst_CMP_ZX, 2, 4 , "CPX" };

	// CPX (Absolute) $EC  3   4
	system->cpu.opcode[0xec] = { inst_CPX_A, 3, 4 , "CPX" };
	// BIT (Absolute) $2c  3   4
	system->cpu.opcode[0x2c] = { inst_BIT_A, 3, 4 , "BIT" };

	// BIT (Zerropage) $24  2   3
	system->cpu.opcode[0x24] = { inst_BIT_Z, 2, 3 , "BIT" };


	// SEI $78  1   2
	system->cpu.opcode[0x78] = { inst_SEI, 1, 2, "SEI" };
	// CLI $58  1   2
	system->cpu.opcode[0x58] = { inst_CLI, 1, 2, "CLI" };
	// CLD $D8  1   2
	system->cpu.opcode[0xd8] = { inst_CLD, 1, 2, "CLD" };
	// CLV $B8  1   2
	system->cpu.opcode[0xb8] = { inst_CLV, 1, 2, "CLV" };
	// SED  $D8  1   2
	system->cpu.opcode[0xF8] = { inst_SED, 1, 2, "SED" };
	// TXS $9A  1   2
	system->cpu.opcode[0x9A] = { inst_TXS, 1, 2, "TXS" };
	// TSX $BA  1   2
	system->cpu.opcode[0xba] = { inst_TSX, 1, 2, "TSX" };
	// INC-SBC $ff  3   2
	system->cpu.opcode[0xff] = { inst_INC_SBC_A, 1, 2, "INC_SBC" };
	// RTI (Implied) 1 6
	system->cpu.opcode[0x40] = { inst_RTI, 1, 6, "RTI" };

#if 0
	FILE* pRomFile;
	errno_t err;

	err = fopen_s(&pRomFile, "D:\\Programmierung\\C64 - SID\\asm\\6502_functional_test.bin", "rb");

	if (err == 0)
		printf("The file 'crt_fopen_s.c' was opened\n");

	fseek(pRomFile, 0, SEEK_END);
	int size = ftell(pRomFile);
	fseek(pRomFile, 0, SEEK_SET);

	fread(&system->bus.mem[0x400], size, 1, pRomFile);

	fclose(pRomFile);
#endif

#if 1
	FILE* pRomFile;
	errno_t err;

	err = fopen_s(&pRomFile, "D:\\Programmierung\\C64 - SID\\asm\\Celestial_Fantasia.prg", "rb");
	
	if (err == 0)
		printf("The file 'crt_fopen_s.c' was opened\n");

	fseek(pRomFile, 0, SEEK_END);
	int size = ftell(pRomFile);
	fseek(pRomFile, 2, SEEK_SET);

	fread(&system->bus.mem[rom_start], size-2, 1, pRomFile);
	
	fclose(pRomFile);

	err = fopen_s(&pRomFile, "D:\\Programmierung\\C64 - SID\\asm\\tiny.o", "rb");

	if (err == 0)
		printf("The file 'crt_fopen_s.c' was opened\n");

	fseek(pRomFile, 0, SEEK_END);
	 size = ftell(pRomFile);
	fseek(pRomFile, 0, SEEK_SET);

	fread(&system->bus.mem[0x0F00], size, 1, pRomFile);
	
	fclose(pRomFile);
#endif

	fkt_count = 0;
	
	for (i = 0; i < 256; i++)
	{
		if (system->cpu.opcode[i].fkt != inst_unknown)
			fkt_count++;
	}
	
	printf("anzahl OPCODES = %d\n", fkt_count);

	system->cpu.reg.sp = 0xff;
	//system->cpu.reg.pc.value = 0xc0ff; // SID paperboy

	system->cpu.reg.pc.value = 0x0F00; // SID stormlord


	//system->cpu.reg.pc.value = 0x400; // TEST
	time_t t;
	srand((unsigned int) time(&t));






#if 1
	err = fopen_s(&pRomFile, "D:\\Programmierung\\C64 - SID\\asm\\memory_2.bin", "wb+");

	if (err == 0)
		printf("The file 'crt_fopen_s.c' was opened\n");

	fwrite(&system->bus.mem[0x00], 0xffff, 1, pRomFile);
	fclose(pRomFile);
#endif
}

void tick_6502_system(struct system* system)
{
	uint8_t  opByte;
	uint16_t pc_old;
	uint32_t i;

	opByte = system->bus.mem[system->cpu.reg.pc.value];

	pc_old = system->cpu.reg.pc.value;
	system->cpu.reg.pc.value = system->cpu.opcode[opByte].fkt(system, &system->bus.mem[system->cpu.reg.pc.value + 1],
															  system->cpu.opcode[opByte].len, system->cpu.opcode[opByte].cyles);
	system->cpu.reg.clocks += system->cpu.opcode[opByte].cyles;
}

uint16_t inst_unknown(struct system *system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	int test = 0;
	return system->cpu.reg.pc.value;
}

uint16_t inst_SEI(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.flags.interrupt = 1;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CLI(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.flags.interrupt = 0;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CLV(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.flags.overflow = 0;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_SED(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.flags.decimal = 1;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CLD(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.flags.decimal = 0;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_TSX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.x = system->cpu.reg.sp;
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_TXS(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.sp = system->cpu.reg.x;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDA_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = opPara[0];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
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
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_AND_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t zpAdr;
	uint16_t addr;
	zpAdr = (system->cpu.reg.x + opPara[0]);
	addr = (system->bus.mem[zpAdr + 1] << 8) + (system->bus.mem[zpAdr]);
	system->cpu.reg.a = system->cpu.reg.a & system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_AND_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t zpAdr;
	uint16_t addr;
	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0]]);
	addr += system->cpu.reg.y;
	system->cpu.reg.a = system->cpu.reg.a & system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ORA_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	// Buggy?
	uint8_t zpAdr;
	uint16_t addr;
	zpAdr = (system->cpu.reg.x + opPara[0]);
	addr = (system->bus.mem[zpAdr + 1] << 8) + (system->bus.mem[zpAdr]);
	system->cpu.reg.a = system->cpu.reg.a | system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ORA_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = system->bus.mem[opPara[0]];
	adr.s.hi = system->bus.mem[(opPara[0] + 1) & 0xff];
	system->cpu.reg.a = system->cpu.reg.a | system->bus.mem[adr.value + system->cpu.reg.y];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDA_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0] + 0]);
	addr += system->cpu.reg.y;
	system->cpu.reg.a = system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CMP_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cmp;
	uint16_t addr;
	addr = (system->bus.mem[opPara[0] + system->cpu.reg.x + 1] << 8) + (system->bus.mem[opPara[0] + system->cpu.reg.x + 0]);
	cmp = system->cpu.reg.a - system->bus.mem[addr];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= system->bus.mem[addr]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CMP_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cmp;
	uint16_t addr;
	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0] + 0]);
	addr += system->cpu.reg.y;
	cmp = system->cpu.reg.a - system->bus.mem[addr];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= system->bus.mem[addr]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDX_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.x = opPara[0];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDA_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->bus.mem[opPara[0]];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDX_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.x = system->bus.mem[opPara[0]];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDX_ZY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.x = system->bus.mem[(opPara[0] + system->cpu.reg.y) & 0xff];

	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDY_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.y = system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];

	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDX_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.x = system->bus.mem[adr.value];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDX_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.x = system->bus.mem[adr.value + system->cpu.reg.y];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDY_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.y = system->bus.mem[adr.value];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STA_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	memory_write(system, opPara[0], system->cpu.reg.a);
	//system->bus.mem[opPara[0]] = system->cpu.reg.a;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STX_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	memory_write(system, opPara[0], system->cpu.reg.x);
	//system->bus.mem[opPara[0]] = system->cpu.reg.x;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STX_ZY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	memory_write(system, (opPara[0] + system->cpu.reg.y) & 0xff, system->cpu.reg.x);
	//system->bus.mem[opPara[0]] = system->cpu.reg.x;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STX_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	memory_write(system, adr.value, system->cpu.reg.x);
	//system->bus.mem[adr.value] = system->cpu.reg.x;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STY_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	memory_write(system, adr.value, system->cpu.reg.y);
	//system->bus.mem[adr.value] = system->cpu.reg.y;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STY_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	memory_write(system, opPara[0], system->cpu.reg.y);

	//system->bus.mem[opPara[0]] = system->cpu.reg.y;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STY_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	memory_write(system, (opPara[0] + system->cpu.reg.x) & 0xff, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_AND_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->cpu.reg.a & opPara[0];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_AND_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.a = (system->cpu.reg.a & system->bus.mem[adr.value]);
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_AND_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.a = (system->cpu.reg.a & system->bus.mem[adr.value + system->cpu.reg.x]);
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_AND_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.a = (system->cpu.reg.a & system->bus.mem[adr.value + system->cpu.reg.y]);
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_AND_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->cpu.reg.a & system->bus.mem[opPara[0]];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_AND_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->cpu.reg.a & system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_INC_SBC_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t tmp;
	union word adr;
	uint16_t add;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	tmp = system->bus.mem[adr.value + system->cpu.reg.x];
	tmp++;
	memory_write(system, adr.value, tmp);
	add = system->cpu.reg.a + (~tmp) + system->cpu.reg.flags.carry;
	system->cpu.reg.a = (uint8_t)add;
	// affects C,N,Z,V Flags
	system->cpu.reg.flags.carry = (add & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	//system->cpu.reg.flags.overflow = TODO
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CLC_IM(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.flags.carry = 0;
	// affects no other Flags
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_SEC_IM(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.flags.carry = 1;
	// affects no other Flags
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ADC_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;

	add_op1 = system->cpu.reg.a;
	add_op2 = opPara[0];
	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;
	system->cpu.reg.a = add_result;

	// affects C,N,Z, V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ADC_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;

	uint8_t zpAdr;
	uint16_t addr;
	zpAdr = (system->cpu.reg.x + opPara[0]);

	addr = (system->bus.mem[zpAdr + 1] << 8) + (system->bus.mem[zpAdr]);

	add_op1 = system->cpu.reg.a;
	add_op2 = system->bus.mem[addr];
	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;
	system->cpu.reg.a = add_result;
	
	// affects C,N,Z, V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);
	
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_SBC_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;

	uint8_t zpAdr;
	uint16_t addr;
	zpAdr = (system->cpu.reg.x + opPara[0]);

	addr = (system->bus.mem[zpAdr + 1] << 8) + (system->bus.mem[zpAdr]);

	add_op1 = system->cpu.reg.a;
	add_op2 = (~system->bus.mem[addr]);
	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;

	system->cpu.reg.a = add_result;

	// affects C,N,Z, V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ADC_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
	

	if (system->cpu.reg.pc.value == 0x35e6)
	{
		int halt = 0;
	}

	add_op1 = system->cpu.reg.a;
	add_op2 = system->bus.mem[opPara[0]];
	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;

	//printf("ADC_Z = add_op1 = %d add_op2 = %d add_result=%d\n", add_op1, add_op2, add_result);

	system->cpu.reg.a = add_result;
	// affects C,N,Z,V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);
	
	//printf("ADC_Z - N %d Z %d V %d C %d\n", system->cpu.reg.flags.negative, system->cpu.reg.flags.zero, system->cpu.reg.flags.overflow, system->cpu.reg.flags.carry);

	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ADC_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;

	add_op1 = system->cpu.reg.a;
	add_op2 = system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];
	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;

	system->cpu.reg.a = add_result;
	// affects C,N,Z,V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);

	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ADC_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
	union word adr;

	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	add_op1 = system->cpu.reg.a;
	add_op2 = system->bus.mem[adr.value];
	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;
	system->cpu.reg.a = add_result;
	
	// affects C,N,Z,V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);

	return system->cpu.reg.pc.value + len;
}

uint16_t inst_SBC_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
	union word adr;

	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	add_op1 = system->cpu.reg.a;
	add_op2 = (~system->bus.mem[adr.value]);
	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;
	system->cpu.reg.a = add_result;

	// affects C,N,Z,V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);

	return system->cpu.reg.pc.value + len;
}

uint16_t inst_SBC_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
	union word adr;

	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	add_op1 = system->cpu.reg.a;
	add_op2 = (~system->bus.mem[adr.value + system->cpu.reg.x]);
	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;
	system->cpu.reg.a = add_result;

	// affects C,N,Z,V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);

	return system->cpu.reg.pc.value + len;
}

uint16_t inst_SBC_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
	union word adr;

	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	add_op1 = system->cpu.reg.a;
	add_op2 = (~system->bus.mem[adr.value + system->cpu.reg.y]);
	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;
	system->cpu.reg.a = add_result;

	// affects C,N,Z,V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);

	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ADC_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
	uint16_t addr;

	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.x;
	
	add_op1 = system->cpu.reg.a;
	add_op2 = system->bus.mem[addr];
	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;
	
	system->cpu.reg.a = add_result;

	// affects C,N,Z,V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);

	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ADC_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
	uint16_t addr;

	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.y;

	add_op1 = system->cpu.reg.a;
	add_op2 = system->bus.mem[addr];
	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;

	system->cpu.reg.a = add_result;

	// affects C,N,Z,V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);

	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CMP_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	uint8_t cmp;

	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.x;
	cmp = system->cpu.reg.a - system->bus.mem[addr];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= system->bus.mem[addr]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CMP_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	uint8_t cmp;

	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.y;
	cmp = system->cpu.reg.a - system->bus.mem[addr];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= system->bus.mem[addr]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ADC_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
	uint16_t addr;
	
	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0] + 0]);
	addr += system->cpu.reg.y;

	add_op1 = system->cpu.reg.a;
	add_op2 = system->bus.mem[addr];

	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;

	system->cpu.reg.a = add_result;
	
	// affects C,N,Z,V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);

	return system->cpu.reg.pc.value + len;
}

uint16_t inst_SBC_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
	uint16_t addr;

	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0] + 0]);
	addr += system->cpu.reg.y;

	add_op1 = system->cpu.reg.a;
	add_op2 = (~system->bus.mem[addr]);

	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;

	system->cpu.reg.a = add_result;

	// affects C,N,Z,V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);

	return system->cpu.reg.pc.value + len;
}

uint16_t inst_SBC_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;

	if (system->cpu.reg.pc.value == 0x35f8)
	{

		int halt = 0;
	}

	add_op1 = system->cpu.reg.a;
	add_op2 = (~system->bus.mem[opPara[0]]);
	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;

	system->cpu.reg.a = (uint8_t)add_result;

	// affects C,N,Z,V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);

	return system->cpu.reg.pc.value + len;
}

uint16_t inst_SBC_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;

	add_op1 = system->cpu.reg.a;
	add_op2 = ~opPara[0];
	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;
	system->cpu.reg.a = (uint8_t)add_result;

	// affects C,N,Z,V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);

	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDY_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.y = opPara[0];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STA_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0] + 0]);
	addr += system->cpu.reg.y;
	
	memory_write(system, addr, system->cpu.reg.a);
	//system->bus.mem[addr] = system->cpu.reg.a;
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

uint16_t inst_JMP_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	union word jmpAdr;

	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	jmpAdr.s.lo = system->bus.mem[adr.value];
	jmpAdr.s.hi = system->bus.mem[(adr.value + 1)];
	return jmpAdr.value;
}

uint16_t inst_STA_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	
	memory_write(system, adr.value, system->cpu.reg.a);
	//system->bus.mem[adr.value] = system->cpu.reg.a;
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
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STA_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t zpAdr;
	uint16_t addr;
	zpAdr = (system->cpu.reg.x + opPara[0]);
	addr = (system->bus.mem[zpAdr + 1] << 8) + (system->bus.mem[zpAdr]);
	
	memory_write(system, addr, system->cpu.reg.a);
	//system->bus.mem[addr] = system->cpu.reg.a;
	// affects no Flags
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_INC_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t newVal;
	uint8_t tmp;

	tmp = system->bus.mem[opPara[0]];
	tmp++;

	memory_write(system, opPara[0], tmp);

//	newVal = system->bus.mem[opPara[0]];

	// affects N,Z Flags
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_INC_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t newVal;
	uint8_t tmp;

	tmp = system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];
	tmp++;

	memory_write(system, (opPara[0] + system->cpu.reg.x) & 0xff, tmp);
	//	newVal = system->bus.mem[opPara[0]];
	// affects N,Z Flags
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_INC_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	uint8_t newVal;
	uint8_t tmp;

	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	
#if 1
	tmp = system->bus.mem[adr.value];
	tmp++;
	memory_write(system, adr.value, tmp);
#endif

	//system->bus.mem[adr.value]++;

	//newVal = system->bus.mem[tmp];
	
	// affects N,Z Flags
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_DEC_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t tmp;
	uint8_t newVal;

	//system->bus.mem[opPara[0]]--;
	tmp = system->bus.mem[opPara[0]];
	tmp--;
	memory_write(system, opPara[0], tmp);
	//newVal = system->bus.mem[opPara[0]];
	// affects N,Z Flags
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_DEC_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t tmp;
	uint8_t newVal;

	//system->bus.mem[opPara[0]]--;
	tmp = system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];
	tmp--;
	memory_write(system, (opPara[0] + system->cpu.reg.x) & 0xff, tmp);
	//newVal = system->bus.mem[opPara[0]];
	// affects N,Z Flags
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_DEC_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t tmp;
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	tmp = system->bus.mem[adr.value];
	tmp--;
	memory_write(system, adr.value, tmp);
	//system->bus.mem[adr.value]--;
	// affects N,Z Flags
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CMP_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cmp;
	cmp = system->cpu.reg.a - opPara[0];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= opPara[0]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CMP_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cmp;
	
	if (system->cpu.reg.pc.value == 0x35f0)
	{
		int h = 0;
	}

	//printf("CMP_Z = a %d; m %d\n", system->cpu.reg.a, system->bus.mem[opPara[0]]);
	cmp = system->cpu.reg.a - system->bus.mem[opPara[0]];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= system->bus.mem[opPara[0]]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CMP_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cmp;
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	cmp = system->cpu.reg.a - system->bus.mem[adr.value];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= system->bus.mem[adr.value]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CPY_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cmp;
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	cmp = system->cpu.reg.y - system->bus.mem[adr.value];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.y >= system->bus.mem[adr.value]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CPY_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cmp;
	cmp = system->cpu.reg.y - opPara[0];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.y >= opPara[0]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CMX_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cmp;
	cmp = system->cpu.reg.x - opPara[0];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.x >= opPara[0]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CPX_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cmp;
	cmp = system->cpu.reg.x - system->bus.mem[opPara[0]];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.x >= system->bus.mem[opPara[0]]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CMP_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cmp;
	cmp = system->cpu.reg.a - system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CPY_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cmp;
	cmp = system->cpu.reg.y - system->bus.mem[opPara[0]];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.y >= system->bus.mem[opPara[0]]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_CPX_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cmp;
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	cmp = system->cpu.reg.x - system->bus.mem[adr.value];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.x >= system->bus.mem[adr.value]);
	update_NZ_Flags(system, cmp);
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

uint16_t inst_BCS(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t newPc;
	newPc = system->cpu.reg.pc.value + len;
	if (system->cpu.reg.flags.carry)
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

uint16_t inst_BVC(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t newPc;
	newPc = system->cpu.reg.pc.value + len;
	if (!system->cpu.reg.flags.overflow)
		newPc += (int8_t)opPara[0];
	// affects no Flags
	return newPc;
}

uint16_t inst_BVS(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t newPc;
	newPc = system->cpu.reg.pc.value + len;
	if (system->cpu.reg.flags.overflow)
		newPc += (int8_t)opPara[0];
	// affects no Flags
	return newPc;
}

uint16_t inst_TAX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.x = system->cpu.reg.a;
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_TXA(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->cpu.reg.x;
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_TYA(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->cpu.reg.y;
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_INY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.y++;
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
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
	update_NZ_Flags(system, system->cpu.reg.y);
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
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_DEY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.y--;
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDA_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	
	addr += system->cpu.reg.x;
	system->cpu.reg.a = system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDY_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.x;
	system->cpu.reg.y = system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_INC_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t tmp;
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.x;
	
#if 1
	tmp = system->bus.mem[addr];
	tmp++;
	memory_write(system, addr, tmp);
#endif

	//system->bus.mem[addr]++;
	
	// affects N,Z Flags
	update_NZ_Flags(system, system->bus.mem[addr]);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_DEC_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t tmp;
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.x;

#if 1
	tmp = system->bus.mem[addr];
	tmp--;
	memory_write(system, addr, tmp);
#endif
	//system->bus.mem[addr]--;
	// affects N,Z Flags
	update_NZ_Flags(system, system->bus.mem[addr]);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDA_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.y;
	system->cpu.reg.a = system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
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
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STA_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.x;
	
	memory_write(system, addr, system->cpu.reg.a);
	//system->bus.mem[addr] = system->cpu.reg.a;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STA_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.y;
	memory_write(system, addr, system->cpu.reg.a);
	//system->bus.mem[addr] = system->cpu.reg.a;
	// no flags change
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LDA_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t addr;
	addr = (opPara[0] + system->cpu.reg.x);
	system->cpu.reg.a = system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_SBC_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;

	add_op1 = system->cpu.reg.a;
	add_op2 = (~system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff]);
	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;
	system->cpu.reg.a = (uint8_t)add_result;

	// affects C,N,Z,V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);
	
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_STA_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t addr;
	addr = (opPara[0] + system->cpu.reg.x);
	
	memory_write(system, addr, system->cpu.reg.a);
	//system->bus.mem[addr] = system->cpu.reg.a;
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
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ASL_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t shiftVal;
	uint8_t tmp;
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	tmp = system->bus.mem[adr.value];
	shiftVal = (tmp << 1);
	tmp <<= 1;

	memory_write(system, adr.value, tmp);

	// affects C,N,Z
	system->cpu.reg.flags.carry = (shiftVal & 0x100) != 0;
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ASL_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t shiftVal;
	uint8_t tmp;
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	tmp = system->bus.mem[adr.value + system->cpu.reg.x];
	shiftVal = (tmp << 1);
	tmp <<= 1;

	memory_write(system, adr.value + system->cpu.reg.x, tmp);

	// affects C,N,Z
	system->cpu.reg.flags.carry = (shiftVal & 0x100) != 0;
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ASL_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t shiftVal;
	uint8_t tmp;

	tmp = system->bus.mem[opPara[0]];
	shiftVal = (tmp << 1);
	tmp <<= 1;
	memory_write(system, opPara[0], tmp);
	// affects C,N,Z
	system->cpu.reg.flags.carry = (shiftVal & 0x100) != 0;
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ASL_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint16_t shiftVal;
	uint8_t tmp;

	tmp = system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];
	shiftVal = (tmp << 1);
	tmp <<= 1;
	memory_write(system, (opPara[0] + system->cpu.reg.x), tmp);
	// affects C,N,Z
	system->cpu.reg.flags.carry = (shiftVal & 0x100) != 0;
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LSR(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t flag_carry;
	flag_carry = system->cpu.reg.a & 0x01;
	system->cpu.reg.a >>= 1;
	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_carry;
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LSR_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t flag_carry;
	uint8_t tmp;
	tmp = system->bus.mem[opPara[0]];
	flag_carry = tmp & 0x01;
	tmp >>= 1;
	memory_write(system, opPara[0], tmp);
	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_carry;
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LSR_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t flag_carry;
	uint8_t tmp;
	tmp = system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];
	flag_carry = tmp & 0x01;
	tmp >>= 1;
	memory_write(system, (opPara[0] + system->cpu.reg.x) & 0xff, tmp);
	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_carry;
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LSR_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t tmp;
	uint8_t flag_carry;
	union word adr;

	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	tmp = system->bus.mem[adr.value];
	flag_carry = tmp & 0x01;
	tmp >>= 1;
	memory_write(system, adr.value, tmp);
	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_carry;
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_LSR_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t tmp;
	uint8_t flag_carry;
	union word adr;

	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	tmp = system->bus.mem[adr.value + system->cpu.reg.x];
	flag_carry = tmp & 0x01;
	tmp >>= 1;
	memory_write(system, adr.value + system->cpu.reg.x, tmp);
	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_carry;
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ROR_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t flag_newCarry;
	uint8_t tmp;
	flag_newCarry = system->bus.mem[opPara[0]] & 0x01;
	
#if 1
	tmp = system->bus.mem[opPara[0]];
	tmp >>= 1;
	tmp |= (system->cpu.reg.flags.carry << 7);
	memory_write(system, opPara[0], tmp);
#endif

	//system->bus.mem[opPara[0]] >>= 1;
	//system->bus.mem[opPara[0]] |= (system->cpu.reg.flags.carry << 7);
	
	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_newCarry;
	update_NZ_Flags(system, system->bus.mem[opPara[0]]);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ROR_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t flag_newCarry;
	uint8_t tmp;
	flag_newCarry = system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff] & 0x01;

#if 1
	tmp = system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];
	tmp >>= 1;
	tmp |= (system->cpu.reg.flags.carry << 7);
	memory_write(system, (opPara[0] + system->cpu.reg.x) & 0xff, tmp);
#endif

	//system->bus.mem[opPara[0]] >>= 1;
	//system->bus.mem[opPara[0]] |= (system->cpu.reg.flags.carry << 7);

	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_newCarry;
	update_NZ_Flags(system, system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff]);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ROR_REG(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t flag_newCarry;
	uint8_t tmp;
	flag_newCarry = system->cpu.reg.a & 0x01;

	system->cpu.reg.a >>= 1;
	system->cpu.reg.a |= (system->cpu.reg.flags.carry << 7);
	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_newCarry;
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ROR_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t flag_newCarry;
	uint8_t tmp;
	union word adr;

	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	flag_newCarry = system->bus.mem[adr.value] & 0x01;

#if 1
	tmp = system->bus.mem[adr.value];
	tmp >>= 1;
	tmp |= (system->cpu.reg.flags.carry << 7);
	memory_write(system, adr.value, tmp);
#endif

	//system->bus.mem[opPara[0]] >>= 1;
	//system->bus.mem[opPara[0]] |= (system->cpu.reg.flags.carry << 7);

	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_newCarry;
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ROR_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t flag_newCarry;
	uint8_t tmp;
	union word adr;

	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	flag_newCarry = system->bus.mem[adr.value + system->cpu.reg.x] & 0x01;

#if 1
	tmp = system->bus.mem[adr.value + system->cpu.reg.x];
	tmp >>= 1;
	tmp |= (system->cpu.reg.flags.carry << 7);
	memory_write(system, adr.value + system->cpu.reg.x, tmp);
#endif

	//system->bus.mem[opPara[0]] >>= 1;
	//system->bus.mem[opPara[0]] |= (system->cpu.reg.flags.carry << 7);

	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_newCarry;
	update_NZ_Flags(system, tmp);
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
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ROL_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t flag_newCarry;
	uint8_t tmp;

	tmp = system->bus.mem[opPara[0]];
	flag_newCarry = (tmp & 0x80) != 0;
	tmp <<= 1;
	tmp |= system->cpu.reg.flags.carry;
	memory_write(system, opPara[0], tmp);

	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_newCarry;
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ROL_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t flag_newCarry;
	uint8_t tmp;

	tmp = system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];
	flag_newCarry = (tmp & 0x80) != 0;
	tmp <<= 1;
	tmp |= system->cpu.reg.flags.carry;
	memory_write(system, (opPara[0] + system->cpu.reg.x) & 0xff, tmp);

	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_newCarry;
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ROL_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t flag_newCarry;
	uint8_t tmp;
	union word adr;

	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	tmp = system->bus.mem[adr.value];
	flag_newCarry = (tmp & 0x80) != 0;
	tmp <<= 1;
	tmp |= system->cpu.reg.flags.carry;
	memory_write(system, adr.value, tmp);

	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_newCarry;
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ROL_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t flag_newCarry;
	uint8_t tmp;
	union word adr;

	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	tmp = system->bus.mem[adr.value + system->cpu.reg.x];
	flag_newCarry = (tmp & 0x80) != 0;
	tmp <<= 1;
	tmp |= system->cpu.reg.flags.carry;
	memory_write(system, adr.value + system->cpu.reg.x, tmp);

	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_newCarry;
	update_NZ_Flags(system, tmp);
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
	// vom stack lesen
	system->cpu.reg.a = system->bus.mem[0x100 + system->cpu.reg.sp];
	// affects N,Z
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_PLP(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cpu_status;
	system->cpu.reg.sp++;
	// vom stack lesen
	cpu_status = system->bus.mem[0x100 + system->cpu.reg.sp];
	//  N | V |	1 | B |	D | I | Z | C
	system->cpu.reg.flags.negative   = ((cpu_status & (1 << 7)) != 0) ? 1 : 0;
	system->cpu.reg.flags.overflow   = ((cpu_status & (1 << 6)) != 0) ? 1 : 0;
	system->cpu.reg.flags.always_one = 1;
	system->cpu.reg.flags.break_flag = ((cpu_status & (1 << 4)) != 0) ? 1 : 0;
	system->cpu.reg.flags.decimal = ((cpu_status & (1 << 3)) != 0) ? 1 : 0;
	system->cpu.reg.flags.interrupt = ((cpu_status & (1 << 2)) != 0) ? 1 : 0;
	system->cpu.reg.flags.zero = ((cpu_status & (1 << 1)) != 0) ? 1 : 0;
	system->cpu.reg.flags.carry = ((cpu_status & (1 << 0)) != 0) ? 1 : 0;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_PHP(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t cpu_status = 0;
	
	if (system->cpu.reg.pc.value == 0x0e83)
	{
		int halt = 0;
	}

	// in stack schreiben
	
	//  N | V |	1 | B |	D | I | Z | C
	if(system->cpu.reg.flags.negative)
		cpu_status |= (1 << 7);

	if(system->cpu.reg.flags.overflow)
		cpu_status |= (1 << 6);

	cpu_status |= (1 << 5); // not_used_flag = 1
	cpu_status |= (1 << 4); // break = 1

	if(system->cpu.reg.flags.decimal)
		cpu_status |= (1 << 3);

	if(system->cpu.reg.flags.interrupt)
		cpu_status |= (1 << 2);

	if(system->cpu.reg.flags.zero)
	   cpu_status |= (1 << 1);

	if(system->cpu.reg.flags.carry)
		cpu_status |= (1 << 0);

	system->bus.mem[0x100 + system->cpu.reg.sp] = cpu_status;
	system->cpu.reg.sp--;

	return system->cpu.reg.pc.value + len;
}

uint16_t inst_TAY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.y = system->cpu.reg.a;
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
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
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ORA_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->cpu.reg.a | system->bus.mem[opPara[0] + system->cpu.reg.x];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ORA_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.a = system->cpu.reg.a | system->bus.mem[adr.value];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ORA_IM(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->cpu.reg.a | opPara[0];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ORA_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.a = system->cpu.reg.a | system->bus.mem[adr.value + system->cpu.reg.x];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_ORA_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.a = system->cpu.reg.a | system->bus.mem[adr.value + system->cpu.reg.y];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_EOR_I(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->cpu.reg.a ^ opPara[0];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_EOR_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->cpu.reg.a ^ system->bus.mem[opPara[0]];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_EOR_IX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	uint8_t zpAdr;
	uint16_t addr;
	addr = (system->bus.mem[opPara[0] + system->cpu.reg.x + 1] << 8) + (system->bus.mem[opPara[0] + system->cpu.reg.x]);
	system->cpu.reg.a = system->cpu.reg.a ^ system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_EOR_ZX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.a = system->cpu.reg.a ^ system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_EOR_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.a = system->cpu.reg.a ^ system->bus.mem[adr.value];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_EOR_AX(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.a = system->cpu.reg.a ^ system->bus.mem[adr.value + system->cpu.reg.x];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_EOR_AY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.a = system->cpu.reg.a ^ system->bus.mem[adr.value + system->cpu.reg.y];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_EOR_IY(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = system->bus.mem[opPara[0]];
	adr.s.hi = system->bus.mem[(opPara[0] + 1) & 0xff];
	system->cpu.reg.a = system->cpu.reg.a ^ system->bus.mem[adr.value + system->cpu.reg.y];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_BIT_A(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.flags.zero = ((system->bus.mem[adr.value] & system->cpu.reg.a) == 0) ? 1: 0;
	system->cpu.reg.flags.negative = ((system->bus.mem[adr.value] & 0x80) != 0) ? 1 : 0;
	system->cpu.reg.flags.overflow = ((system->bus.mem[adr.value] & 0x40) != 0) ? 1 : 0;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_BIT_Z(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	system->cpu.reg.flags.zero = ((system->bus.mem[opPara[0]] & system->cpu.reg.a) == 0) ? 1 : 0;
	system->cpu.reg.flags.negative = ((system->bus.mem[opPara[0]] & 0x80) != 0) ? 1 : 0;
	system->cpu.reg.flags.overflow = ((system->bus.mem[opPara[0]] & 0x40) != 0) ? 1 : 0;
	return system->cpu.reg.pc.value + len;
}

uint16_t inst_RTI(struct system* system, uint8_t* opPara, uint32_t len, uint32_t cyles)
{
	union word adr;
	uint8_t cpu_status;

	system->cpu.reg.sp++;
	// vom stack lesen
	cpu_status = system->bus.mem[0x100 + system->cpu.reg.sp];
	//  N | V |	1 | B |	D | I | Z | C
	system->cpu.reg.flags.negative = ((cpu_status & (1 << 7)) != 0) ? 1 : 0;
	system->cpu.reg.flags.overflow = ((cpu_status & (1 << 6)) != 0) ? 1 : 0;
	system->cpu.reg.flags.always_one = 1;
	system->cpu.reg.flags.break_flag = ((cpu_status & (1 << 4)) != 0) ? 1 : 0;
	system->cpu.reg.flags.decimal = ((cpu_status & (1 << 3)) != 0) ? 1 : 0;
	system->cpu.reg.flags.interrupt = ((cpu_status & (1 << 2)) != 0) ? 1 : 0;
	system->cpu.reg.flags.zero = ((cpu_status & (1 << 1)) != 0) ? 1 : 0;
	system->cpu.reg.flags.carry = ((cpu_status & (1 << 0)) != 0) ? 1 : 0;
	
	// vom Stack holen
	system->cpu.reg.sp++;
	adr.s.lo = system->bus.mem[0x100 + system->cpu.reg.sp];
	system->cpu.reg.sp++;
	adr.s.hi = system->bus.mem[0x100 + system->cpu.reg.sp];

	return adr.value;
}

void update_NZ_Flags(struct system* system, uint8_t value)
{
	system->cpu.reg.flags.negative = (value & 0x80) != 0;
	system->cpu.reg.flags.zero = (value == 0);
}

void update_V_Flag(struct system* system, uint8_t add_op1 , uint8_t add_op2, uint8_t add_result)
{
#if 0
	if ((add_op1 & 0x80) == (add_op2 & 0x80))
	{
		if ((add_result & 0x80) != (add_op1 & 0x80))
			system->cpu.reg.flags.overflow = 1;
		else system->cpu.reg.flags.overflow = 0;
	} else system->cpu.reg.flags.overflow = 0;
#endif

	// A = Accumulator before the addition
	// v = the value adding to the accumulator
	// s = the sum of the addition (A+v+C)

	if ((add_op1 ^ add_result) & (add_op2 ^ add_result) & 0x80)
		system->cpu.reg.flags.overflow = 1;
	else
		system->cpu.reg.flags.overflow = 0;

}

void memory_write(struct system* system, uint16_t addr, uint8_t value)
{
	
	system->bus.mem[addr] = value;

#if 0
	//if(addr == 0xd020)
	// write_emu_u8(s, addr, value);
#endif

#if 1
	if ((addr >= 0xd400) && (addr <= 0xd7ff))
		write_emu_u8(s, addr, value);
	//else printf("write = %04X:%d\n", addr, value);
#endif
}
