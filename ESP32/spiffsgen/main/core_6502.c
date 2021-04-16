#include "core_6502.h"
#include <stdio.h>
#include <memory.h>
#include <lwip/sockets.h>
#include "driver/gpio.h"
#include "esp_task_wdt.h"

#include "esp_timer.h"
#include "xtensa/core-macros.h"

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <memory.h>
#include <time.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#endif

extern int s;

inline void update_NZ_Flags(struct system* system, uint8_t value);
inline void update_V_Flag(struct system* system, uint8_t add_op1, uint8_t add_op2, uint8_t add_result);

void memory_write(struct system* system, uint16_t addr, uint8_t value);

uint16_t inst_LDA_I(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LDA_IX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ORA_IX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LDA_IY(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LDA_Z(struct system *system, uint8_t *opPara, uint32_t len);
uint16_t inst_LDA_ZX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_STA_ZX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LDA_AX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LDY_AX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_INC_AX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LDA_AY(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_STA_AY(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_STA_AX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LDX_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LDX_ZY(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LDY_ZX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LDX_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LDX_AY(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LDY_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LDY_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LDX_I(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_STA_Z(struct system *system, uint8_t *opPara, uint32_t len);
uint16_t inst_STX_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_STX_ZY(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_STX_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_STY_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_STY_ZX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_STY_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_STA_IX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_STA_IY(struct system *system, uint8_t *opPara, uint32_t len);
uint16_t inst_AND_I(struct system *system, uint8_t *opPara, uint32_t len);
uint16_t inst_AND_IX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_AND_IY(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_AND_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_AND_AX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_AND_AY(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CLC_IM(struct system *system, uint8_t *opPara, uint32_t len);
uint16_t inst_SEC_IM(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ADC_I(struct system *system, uint8_t *opPara, uint32_t len);

uint16_t inst_ADC_IX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ADC_IY(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_ADC_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ADC_ZX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ADC_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_SBC_A(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_SBC_AX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_SBC_AY(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_SBC_IX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_SBC_IY(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_ADC_AX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ADC_AY(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_LDY_I(struct system *system, uint8_t *opPara, uint32_t len);
uint16_t inst_JMP_A(struct system *system, uint8_t *opPara, uint32_t len);
uint16_t inst_JMP_I(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_STA_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LDA_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_INC_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_INC_ZX(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_INC_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_DEC_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_DEC_ZX(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_DEC_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_DEC_AX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CMP_I(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CMP_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CMP_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CPY_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CMP_IY(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CMP_IX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CMP_AX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CMP_AY(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_CPY_I(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CMX_I(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CPX_Z(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_CMP_ZX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CPX_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CPY_ZX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CPY_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_BNE(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_BCC(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_BCS(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_BPL(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_BMI(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_BEQ(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_BVC(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_BVS(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_TAX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_TXA(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_TYA(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_TAY(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_INY(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_JSR(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_RTS(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_DEX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_INX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_DEY(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ASL(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ASL_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ASL_AX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ASL_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ASL_ZX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LSR(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LSR_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LSR_ZX(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_LSR_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_LSR_AX(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_ROL(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ROL_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ROL_ZX(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_ROL_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ROL_AX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ROR_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ROR_ZX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ROR_REG(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ROR_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ROR_AX(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_SBC_I(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_SBC_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_SBC_ZX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ORA_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ORA_ZX(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_ORA_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ORA_IM(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ORA_AX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ORA_AY(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_ORA_IY(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_EOR_I(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_EOR_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_EOR_IX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_EOR_ZX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_EOR_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_EOR_AX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_EOR_AY(struct system* system, uint8_t* opPara, uint32_t len);

uint16_t inst_EOR_IY(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_AND_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_AND_ZX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_NOP(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_PHA(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_PLA(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_PLP(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_PHP(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_ADC_IY(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_BIT_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_BIT_Z(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_SEI(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CLI(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CLV(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_SED(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_CLD(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_TXS(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_TSX(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_INC_SBC_A(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_RTI(struct system* system, uint8_t* opPara, uint32_t len);
uint16_t inst_unknown(struct system* system, uint8_t *opPara, uint32_t len);

void init_6502_sytem(struct system *system)
{
	uint32_t i;
	uint32_t fkt_count;

	memset(system, 0x00, sizeof(struct system));

	for (i = 0; i < 256; i++)
		system->cpu.opcode[i].fkt = inst_unknown;

	// ORA (Zero Page) 0x05 LEN 2 TIM 3 OK
	system->cpu.opcode[0x05].fkt = inst_ORA_Z;
	system->cpu.opcode[0x05].len = 2;
	system->cpu.opcode[0x05].cyles = 3;
	// ORA (Zero Page,X) 0x15 LEN 2 TIM 4 OK
	system->cpu.opcode[0x15].fkt = inst_ORA_ZX;
	system->cpu.opcode[0x15].len = 2;
	system->cpu.opcode[0x15].cyles = 4;
	// ORA (Absolute) 0x0d LEN 3 TIM 4 OK
	system->cpu.opcode[0x0d].fkt = inst_ORA_A;
	system->cpu.opcode[0x0d].len = 3;
	system->cpu.opcode[0x0d].cyles = 4;
	// ORA (Implied) 0x09 LEN 2 TIM 2 OK
	system->cpu.opcode[0x09].fkt = inst_ORA_IM;
	system->cpu.opcode[0x09].len = 2;
	system->cpu.opcode[0x09].cyles = 2;
	// ORA (Absolute,X) 0x1d LEN 3 TIM 4 OK
	system->cpu.opcode[0x1d].fkt = inst_ORA_AX;
	system->cpu.opcode[0x1d].len = 3;
	system->cpu.opcode[0x1d].cyles = 4;//+
	// ORA (Absolute,Y) 0x19 LEN 3 TIM 4 OK
	system->cpu.opcode[0x19].fkt = inst_ORA_AY;
	system->cpu.opcode[0x19].len = 3;
	system->cpu.opcode[0x19].cyles = 4; // +
	// LDY  (Immidiate) 0xa0 LEN 2 TIM 2 OK
	system->cpu.opcode[0xa0].fkt = inst_LDY_I;
	system->cpu.opcode[0xa0].len = 2;
	system->cpu.opcode[0xa0].cyles = 2;
	// CLC  (Implied)   0x18 LEN 1 TIM 2 OK
	system->cpu.opcode[0x18].fkt = inst_CLC_IM;
	system->cpu.opcode[0x18].len = 1;
	system->cpu.opcode[0x18].cyles = 2;
	// AND  (Immidiate) 0x29 LEN 2 TIM 2 OK
	system->cpu.opcode[0x29].fkt = inst_AND_I;
	system->cpu.opcode[0x29].len = 2;
	system->cpu.opcode[0x29].cyles = 2;
	// AND  (Indirect, X) 0x21 LEN 2 TIM 6 OK
	system->cpu.opcode[0x21].fkt = inst_AND_IX;
	system->cpu.opcode[0x21].len = 2;
	system->cpu.opcode[0x21].cyles = 6;
	// AND  (Indirect, Y) 0x31 LEN 2 TIM 5 OK
	system->cpu.opcode[0x31].fkt = inst_AND_IY;
	system->cpu.opcode[0x31].len = 2;
	system->cpu.opcode[0x31].cyles = 5;//+
	// AND  (Absolute) 0x2d LEN 3 TIM 4  OK
	system->cpu.opcode[0x2d].fkt = inst_AND_A;
	system->cpu.opcode[0x2d].len = 3;
	system->cpu.opcode[0x2d].cyles = 4;
	// AND  (Absolute,Y) 0x39 LEN 3 TIM 4 OK
	system->cpu.opcode[0x39].fkt = inst_AND_AY;
	system->cpu.opcode[0x39].len = 3;
	system->cpu.opcode[0x39].cyles = 4;//+
	// AND  (Absolute,X) 0x3d LEN 3 TIM 4 OK
	system->cpu.opcode[0x3d].fkt = inst_AND_AX;
	system->cpu.opcode[0x3d].len = 3;
	system->cpu.opcode[0x3d].cyles = 4;//+
    // JMP  (Absolute) 0x4C LEN 3 TIM 3 OK
	system->cpu.opcode[0x4c].fkt = inst_JMP_A;
	system->cpu.opcode[0x4c].len = 3;
	system->cpu.opcode[0x4c].cyles = 3;
	// JMP  (Indirekt) 0x6C LEN 3 TIM 3 OK
	system->cpu.opcode[0x6c].fkt = inst_JMP_I;
	system->cpu.opcode[0x6c].len = 3;
	system->cpu.opcode[0x6c].cyles = 5;
	// ADC  (Immidiate) 0x69 LEN 2 TIM 2 OK
	system->cpu.opcode[0x69].fkt = inst_ADC_I;
	system->cpu.opcode[0x69].len = 2;
	system->cpu.opcode[0x69].cyles = 2;
	// ADC  (Indirekt, X) 0x61 LEN 2 TIM 6 OK
	system->cpu.opcode[0x61].fkt = inst_ADC_IX;
	system->cpu.opcode[0x61].len = 2;
	system->cpu.opcode[0x61].cyles = 6;
	// ADC  (Indirekt, Y) 0x71 LEN 2 TIM 6 OK
	system->cpu.opcode[0x71].fkt = inst_ADC_IY;
	system->cpu.opcode[0x71].len = 2;
	system->cpu.opcode[0x71].cyles = 5;//+
	// ADC  (Zerro Page) 0x65 LEN 2 TIM 3 OK
	system->cpu.opcode[0x65].fkt = inst_ADC_Z;
	system->cpu.opcode[0x65].len = 2;
	system->cpu.opcode[0x65].cyles = 3;
    // ADC  (Zerro Page,X) 0x75 LEN 2 TIM 4	OK		  
    system->cpu.opcode[0x75].fkt = inst_ADC_ZX;
	system->cpu.opcode[0x75].len = 2;
	system->cpu.opcode[0x75].cyles = 4;
	// ADC  (Absolute) 0x6d LEN 3 TIM 4 OK
    system->cpu.opcode[0x6d].fkt = inst_ADC_A;
	system->cpu.opcode[0x6d].len = 3;
	system->cpu.opcode[0x6d].cyles = 4;
    // ADC  (Absolute,X) 0x7d LEN 3 TIM 4 OK
	system->cpu.opcode[0x7d].fkt = inst_ADC_AX;
	system->cpu.opcode[0x7d].len = 3;
	system->cpu.opcode[0x7d].cyles = 4;//+
	// ADC  (Absolute,Y) 0x79 LEN 3 TIM 4 
	system->cpu.opcode[0x79].fkt = inst_ADC_AY;
	system->cpu.opcode[0x79].len = 3;
	system->cpu.opcode[0x79].cyles = 4;//+
	// LDA  (Indirect,X) 0xa1 LEN 2 TIM 2 
	system->cpu.opcode[0xa1].fkt = inst_LDA_IX;
	system->cpu.opcode[0xa1].len = 2;
	system->cpu.opcode[0xa1].cyles = 6;
	// LDX  (Immidiate) 0xa2 LEN 2 TIM 2 
	system->cpu.opcode[0xa2].fkt = inst_LDX_I;
	system->cpu.opcode[0xa2].len = 2;
	system->cpu.opcode[0xa2].cyles = 2;
    // LDA  (Zero Page) 0xa5 LEN 2 TIM 3 
	system->cpu.opcode[0xa5].fkt = inst_LDA_Z;
	system->cpu.opcode[0xa5].len = 2;
	system->cpu.opcode[0xa5].cyles = 3;
    // LDX  (Zero Page) 0xa6 LEN 2 TIM 3 
    system->cpu.opcode[0xa6].fkt = inst_LDX_Z;
	system->cpu.opcode[0xa6].len = 2;
	system->cpu.opcode[0xa6].cyles = 3;
	// LDX  (Zero Page, Y) 0xb6 LEN 2 TIM 4 OK
	system->cpu.opcode[0xb6].fkt = inst_LDX_ZY;
	system->cpu.opcode[0xb6].len = 2;
	system->cpu.opcode[0xb6].cyles = 4;
	// LDY  (Zero Page, X) 0xb4 LEN 2 TIM 4 OK
	system->cpu.opcode[0xb4].fkt = inst_LDY_ZX;
	system->cpu.opcode[0xb4].len = 2;
	system->cpu.opcode[0xb4].cyles = 4;
    // LDX  (Absolute) 0xae LEN 3 TIM 4 OK
	system->cpu.opcode[0xae].fkt = inst_LDX_A;
	system->cpu.opcode[0xae].len = 3;
	system->cpu.opcode[0xae].cyles = 4;
	// LDX  (Absolute,Y) 0xbe LEN 3 TIM 4 OK
	system->cpu.opcode[0xbe].fkt = inst_LDX_AY; 
	system->cpu.opcode[0xbe].len = 3;
	system->cpu.opcode[0xbe].cyles = 4;//+
	// LDY  (Absolute) 0xac LEN 3 TIM 4 OK
	system->cpu.opcode[0xac].fkt = inst_LDY_A;
	system->cpu.opcode[0xac].len = 3;
	system->cpu.opcode[0xac].cyles = 4;
	// DEC  (Zero Page) 0xc6 LEN 2 TIM 5 OK
	system->cpu.opcode[0xc6].fkt = inst_DEC_Z;
	system->cpu.opcode[0xc6].len = 2;
	system->cpu.opcode[0xc6].cyles = 5;
	// DEC  (Zero Page,X) 0xD6 LEN 2 TIM 6 OK
	system->cpu.opcode[0xd6].fkt = inst_DEC_ZX;
	system->cpu.opcode[0xd6].len = 2;
	system->cpu.opcode[0xd6].cyles = 6;
    // DEC  (Absolute) 0xce LEN 3 TIM 6 OK
    system->cpu.opcode[0xce].fkt = inst_DEC_A;
	system->cpu.opcode[0xce].len = 3;
	system->cpu.opcode[0xce].cyles = 6;
    // DEC  (Absolute,X) 0xde LEN 3 TIM 7 OK
	system->cpu.opcode[0xde].fkt = inst_DEC_AX;
	system->cpu.opcode[0xde].len = 3;
	system->cpu.opcode[0xde].cyles = 7;
	// LDA  (Immediate) 0xa9 LEN 2 TIM 2 OK
	system->cpu.opcode[0xa9].fkt = inst_LDA_I;
	system->cpu.opcode[0xa9].len = 2;
	system->cpu.opcode[0xa9].cyles = 2;
	// LDA  (Absolute,X) 0xbd LEN 3 TIM 4 OK
	system->cpu.opcode[0xbd].fkt = inst_LDA_AX;
	system->cpu.opcode[0xbd].len = 3;
	system->cpu.opcode[0xbd].cyles = 4;//+
    // LDA  (Absolute,Y) 0xb9 LEN 3 TIM 4 OK
	system->cpu.opcode[0xb9].fkt = inst_LDA_AY;
	system->cpu.opcode[0xb9].len = 3;
	system->cpu.opcode[0xb9].cyles = 4;//+
	// CMP  (Immediate) 0xc9 LEN 2 TIM 2 OK
	system->cpu.opcode[0xc9].fkt = inst_CMP_I;
	system->cpu.opcode[0xc9].len = 2;
	system->cpu.opcode[0xc9].cyles = 2;
	// CMP  (Zero Page) 0xc5 LEN 2 TIM 3 OK
	system->cpu.opcode[0xc5].fkt = inst_CMP_Z;
	system->cpu.opcode[0xc5].len = 2;
	system->cpu.opcode[0xc5].cyles = 3;
	// CMP  (Absolute) 0xcd LEN 3 TIM 4 OK
	system->cpu.opcode[0xcd].fkt = inst_CMP_A;
	system->cpu.opcode[0xcd].len = 3;
	system->cpu.opcode[0xcd].cyles = 4;
    // CPY  (Absolute) 0xcc LEN 3 TIM 4 OK
    system->cpu.opcode[0xcc].fkt = inst_CPY_A;
	system->cpu.opcode[0xcc].len = 3;
	system->cpu.opcode[0xcc].cyles = 4;
	// CMP  (Absolute,X) 0xdd LEN 3 TIM 4 OK
    system->cpu.opcode[0xdd].fkt = inst_CMP_AX;
	system->cpu.opcode[0xdd].len = 3;
	system->cpu.opcode[0xdd].cyles = 4;//+
	// CMP  (Absolute,Y) 0xd9 LEN 3 TIM 4 OK
    system->cpu.opcode[0xd9].fkt = inst_CMP_AY;
	system->cpu.opcode[0xd9].len = 3;
	system->cpu.opcode[0xd9].cyles = 4;//+
	// CPY  (Immediate) 0xc0 LEN 2 TIM 2 OK
    system->cpu.opcode[0xc0].fkt = inst_CPY_I;
	system->cpu.opcode[0xc0].len = 2;
	system->cpu.opcode[0xc0].cyles = 2;
    // CPY  (Zerropage) 0xc4 LEN 2 TIM 3 OK
    system->cpu.opcode[0xc4].fkt = inst_CPY_Z;
	system->cpu.opcode[0xc4].len = 2;
	system->cpu.opcode[0xc4].cyles = 3;
	// CMX  (Immediate) 0xe0 LEN 2 TIM 2 OK
    system->cpu.opcode[0xe0].fkt = inst_CMX_I;
	system->cpu.opcode[0xe0].len = 2;
	system->cpu.opcode[0xe0].cyles = 2;
	// BNE  (Branch not Equal) 0xd0 LEN 2 TIM 2 OK
	system->cpu.opcode[0xd0].fkt = inst_BNE;
	system->cpu.opcode[0xd0].len = 2;
	system->cpu.opcode[0xd0].cyles = 2;//+1 branch takes + 1 256 page crossed
	// BMI  (Branch on MInus) 0x30 LEN 2 TIM 2 OK
	system->cpu.opcode[0x30].fkt = inst_BMI;
	system->cpu.opcode[0x30].len = 2;
	system->cpu.opcode[0x30].cyles = 2;//+1 branch takes + 1 256 page crossed
	// BEQ  (Branch On Equal) 0xf0 LEN 2 TIM 2 OK
	system->cpu.opcode[0xf0].fkt = inst_BEQ;
	system->cpu.opcode[0xf0].len = 2;
	system->cpu.opcode[0xf0].cyles = 2;//+1 branch takes + 1 256 page crossed
	// BCC  (Branch on Carry Clear) 0x90 LEN 2 TIM 2 OK
	system->cpu.opcode[0x90].fkt = inst_BCC;
	system->cpu.opcode[0x90].len = 2;
	system->cpu.opcode[0x90].cyles = 2;//+1 branch takes + 1 256 page crossed
	// BCS  (Branch on Carry Set) 0xB0 LEN 2 TIM 2 OK
	system->cpu.opcode[0xB0].fkt = inst_BCS;
	system->cpu.opcode[0xB0].len = 2;
	system->cpu.opcode[0xB0].cyles = 2; //+1 branch takes + 1 256 page crossed
	// STA  (Indirect, X) 0x81 LEN 2 TIM 6 OK
	system->cpu.opcode[0x81].fkt = inst_STA_IX;
	system->cpu.opcode[0x81].len = 2;
	system->cpu.opcode[0x81].cyles = 6;
	// STA  (Zero Page) 0x85 LEN 2 TIM 3 OK
	system->cpu.opcode[0x85].fkt = inst_STA_Z;
	system->cpu.opcode[0x85].len = 2;
	system->cpu.opcode[0x85].cyles = 3;
	// STA  (Immidiate Y) 0x91 LEN 2 TIM 6 OK
	system->cpu.opcode[0x91].fkt = inst_STA_IY;
	system->cpu.opcode[0x91].len = 2;
	system->cpu.opcode[0x91].cyles = 6;
	// LDA  (Indirect, Y) 0xB1 LEN 2 TIM 5 OK
    system->cpu.opcode[0xb1].fkt = inst_LDA_IY;
	system->cpu.opcode[0xb1].len = 2;
	system->cpu.opcode[0xb1].cyles = 5;//+1
	// TAX  (Transfer A to X) 0xaa LEN 1 TIM 2 OK
    system->cpu.opcode[0xaa].fkt = inst_TAX;
	system->cpu.opcode[0xaa].len = 1;
	system->cpu.opcode[0xaa].cyles = 2;
	// TXA  (Transfer X to A) 0x8a LEN 1 TIM 2 OK
    system->cpu.opcode[0x8a].fkt = inst_TXA;
	system->cpu.opcode[0x8a].len = 1;
	system->cpu.opcode[0x8a].cyles = 2;
	// TYA  (Transfer Y to A) 0x98 LEN 1 TIM 2 OK
    system->cpu.opcode[0x98].fkt = inst_TYA;
	system->cpu.opcode[0x98].len = 1;
	system->cpu.opcode[0x98].cyles = 2;
    // INY  (INcrement Y) 0xc8 LEN 1 TIM 2 OK
	system->cpu.opcode[0xc8].fkt = inst_INY;
	system->cpu.opcode[0xc8].len = 1;
	system->cpu.opcode[0xc8].cyles = 2;
	// JSR  (Jump to SubRoutine) 0x20 LEN 3 TIM 6 OK
	system->cpu.opcode[0x20].fkt = inst_JSR;
	system->cpu.opcode[0x20].len = 3;
	system->cpu.opcode[0x20].cyles = 6;
	// STY  (Zero Page) 0x84 LEN 2 TIM 3 OK
	system->cpu.opcode[0x84].fkt = inst_STY_Z;
	system->cpu.opcode[0x84].len = 2;
	system->cpu.opcode[0x84].cyles = 3;
	// STY  (Zero Page, X) 0x94 LEN 2 TIM 4 OK
    system->cpu.opcode[0x94].fkt = inst_STY_ZX;
	system->cpu.opcode[0x94].len = 2;
	system->cpu.opcode[0x94].cyles = 4;
	// STY  (Absolute) 0x8c LEN 2 TIM 4 OK
    system->cpu.opcode[0x8c].fkt = inst_STY_A;
	system->cpu.opcode[0x8c].len = 3;
	system->cpu.opcode[0x8c].cyles = 4;
	// STX  (Zero Page) 0x86 LEN 2 TIM 3 OK
    system->cpu.opcode[0x86].fkt = inst_STX_Z;
	system->cpu.opcode[0x86].len = 2;
	system->cpu.opcode[0x86].cyles = 3;
	// STX  (Zero Page,Y) 0x96 LEN 2 TIM 4 OK
    system->cpu.opcode[0x96].fkt = inst_STX_ZY;
	system->cpu.opcode[0x96].len = 2;
	system->cpu.opcode[0x96].cyles = 4;
	// STX  (Absolute) 0x8e LEN 3 TIM 4 OK
    system->cpu.opcode[0x8e].fkt = inst_STX_A;
	system->cpu.opcode[0x8e].len = 3;
	system->cpu.opcode[0x8e].cyles = 4;
	// LDY  (Zero Page) 0xa4 LEN 2 TIM 3 OK
    system->cpu.opcode[0xa4].fkt = inst_LDY_Z;
	system->cpu.opcode[0xa4].len = 2;
	system->cpu.opcode[0xa4].cyles = 3;
	// RTS  (ReTurn from Subroutine) 0x60 LEN 1 TIM 6 OK
	system->cpu.opcode[0x60].fkt = inst_RTS;
	system->cpu.opcode[0x60].len = 1;
	system->cpu.opcode[0x60].cyles = 6;
	// DEX  (DEcrement X) 0xca LEN 1 TIM 2 OK
    system->cpu.opcode[0xca].fkt = inst_DEX;
	system->cpu.opcode[0xca].len = 1;
	system->cpu.opcode[0xca].cyles = 2;
	// DEY  (DEcrement Y) 0x88 LEN 1 TIM 2 OK
    system->cpu.opcode[0x88].fkt = inst_DEY;
	system->cpu.opcode[0x88].len = 1;
	system->cpu.opcode[0x88].cyles = 2;
	// BPL  (Branch On Equal) 0x10 LEN 2 TIM 2 OK
    system->cpu.opcode[0x10].fkt = inst_BPL;
	system->cpu.opcode[0x10].len = 2;
	system->cpu.opcode[0x10].cyles = 2; //+1 branch takes + 1 256 page crossed
	// INX  (INcrement X) 0xe8 LEN 1 TIM 2 OK
    system->cpu.opcode[0xe8].fkt = inst_INX;
	system->cpu.opcode[0xe8].len = 1;
	system->cpu.opcode[0xe8].cyles = 2;
	// INC  (Zero Page) 0xe6 LEN 2 TIM 5 OK
	system->cpu.opcode[0xe6].fkt = inst_INC_Z;
	system->cpu.opcode[0xe6].len = 2;
	system->cpu.opcode[0xe6].cyles = 5;
	// INC  (Zero Page,X) 0xf6 LEN 2 TIM 6 OK
	system->cpu.opcode[0xf6].fkt = inst_INC_ZX;
	system->cpu.opcode[0xf6].len = 2;
	system->cpu.opcode[0xf6].cyles = 6;
	// INC  (Absolute) 0xee LEN 3 TIM 6 OK
	system->cpu.opcode[0xee].fkt = inst_INC_A;
	system->cpu.opcode[0xee].len = 3;
	system->cpu.opcode[0xee].cyles = 6;
	// STA  (Absolute,X) 0x9d LEN 3 TIM 5 OK
	system->cpu.opcode[0x9d].fkt = inst_STA_AX;
	system->cpu.opcode[0x9d].len = 3;
	system->cpu.opcode[0x9d].cyles = 5;
	// STA  (Absolute,Y) 0x99 LEN 3 TIM 5 OK
	system->cpu.opcode[0x99].fkt = inst_STA_AY;
	system->cpu.opcode[0x99].len = 3;
	system->cpu.opcode[0x99].cyles = 5;
	// STA  (Zero Page, X) 0x95 LEN 2 TIM 4 OK
	system->cpu.opcode[0x95].fkt = inst_STA_ZX;
	system->cpu.opcode[0x95].len = 2;
	system->cpu.opcode[0x95].cyles = 4;
	// LDA (Zero Page, X) 0xb5 LEN 2 TIM 4 OK
	system->cpu.opcode[0xb5].fkt = inst_LDA_ZX;
	system->cpu.opcode[0xb5].len = 2;
	system->cpu.opcode[0xb5].cyles = 4;
	// ASL  (Arithmetic Shift Left) 0x0a LEN 1 TIM 2 OK
	system->cpu.opcode[0x0a].fkt = inst_ASL;
	system->cpu.opcode[0x0a].len = 1;
	system->cpu.opcode[0x0a].cyles = 2;
	// ASL  (Absolute) 0x0e LEN 3 TIM 6 OK
	system->cpu.opcode[0x0e].fkt = inst_ASL_A;
	system->cpu.opcode[0x0e].len = 3;
	system->cpu.opcode[0x0e].cyles = 6;
	// ASL  (Absolute,X) 0x1e LEN 3 TIM 7 OK
	system->cpu.opcode[0x1e].fkt = inst_ASL_AX;
	system->cpu.opcode[0x1e].len = 3;
	system->cpu.opcode[0x1e].cyles = 7;
    // ASL  (Arithmetic Shift Left, ZerroPage) 0x06 LEN 2 TIM 5 OK
	system->cpu.opcode[0x06].fkt = inst_ASL_Z;
	system->cpu.opcode[0x06].len = 2;
	system->cpu.opcode[0x06].cyles = 5;
	// ASL  (ZerroPage,X) 0x16 LEN 2 TIM 6 OK
	system->cpu.opcode[0x16].fkt = inst_ASL_ZX;
	system->cpu.opcode[0x16].len = 2;
	system->cpu.opcode[0x16].cyles = 6;
	// TAY  (Transfer A to Y) 0xa8 LEN 1 TIM 2 OK
	system->cpu.opcode[0xa8].fkt = inst_TAY;
	system->cpu.opcode[0xa8].len = 1;
	system->cpu.opcode[0xa8].cyles = 2;
	// SBC (Zerropage) 0xe5 LEN 2 TIM 3 OK
	system->cpu.opcode[0xe5].fkt = inst_SBC_Z;
	system->cpu.opcode[0xe5].len = 2;
	system->cpu.opcode[0xe5].cyles = 3;
	// SBC (Immediate) 0xe9 LEN 2 TIM 2 OK
	system->cpu.opcode[0xe9].fkt = inst_SBC_I;
	system->cpu.opcode[0xe9].len = 2;
	system->cpu.opcode[0xe9].cyles = 2;
	// SBC (Indirekt,X) 0xe1 LEN 2 TIM 6 OK
	system->cpu.opcode[0xe1].fkt = inst_SBC_IX;
	system->cpu.opcode[0xe1].len = 2;
	system->cpu.opcode[0xe1].cyles = 6;
	// SBC (Indirekt,Y) 0xf1 LEN 2 TIM 5 OK
	system->cpu.opcode[0xf1].fkt = inst_SBC_IY;
	system->cpu.opcode[0xf1].len = 2;
	system->cpu.opcode[0xf1].cyles = 5;//+
	// SBC (Absolute) 0xed LEN 3 TIM 4 OK
	system->cpu.opcode[0xed].fkt = inst_SBC_A;
	system->cpu.opcode[0xed].len = 3;
	system->cpu.opcode[0xed].cyles = 4;
	// SBC (Absolute,X) 0xfd LEN 3 TIM 4  OK
	system->cpu.opcode[0xfd].fkt = inst_SBC_AX;
	system->cpu.opcode[0xfd].len = 3;
	system->cpu.opcode[0xfd].cyles = 4;//+
	// SBC (Absolute,Y) 0xf9 LEN 3 TIM 4 OK
	system->cpu.opcode[0xf9].fkt = inst_SBC_AY;
	system->cpu.opcode[0xf9].len = 3;
	system->cpu.opcode[0xf9].cyles = 4;//+
	// SEC (SEt Carry) 0x38 LEN 1 TIM 2 OK
	system->cpu.opcode[0x38].fkt = inst_SEC_IM;
	system->cpu.opcode[0x38].len = 1;
	system->cpu.opcode[0x38].cyles = 2;
	// LSR (Logical Shift Right) 0x4a LEN 1 TIM 2 OK
	system->cpu.opcode[0x4a].fkt = inst_LSR;
	system->cpu.opcode[0x4a].len = 1;
	system->cpu.opcode[0x4a].cyles = 2;
	// LSR (Logical Shift Right, ZerroPage) 0x46 LEN 2 TIM 5 OK
	system->cpu.opcode[0x46].fkt = inst_LSR_Z;
	system->cpu.opcode[0x46].len = 2;
	system->cpu.opcode[0x46].cyles = 5;
	// LSR (ZerroPage,X) 0x56 LEN 2 TIM 5 OK
	system->cpu.opcode[0x56].fkt = inst_LSR_ZX;
	system->cpu.opcode[0x56].len = 2;
	system->cpu.opcode[0x56].cyles = 6;
    // LSR (Logical Shift Right) 0x4e LEN 3 TIM 6 OK
    system->cpu.opcode[0x4e].fkt = inst_LSR_A;
	system->cpu.opcode[0x4e].len = 3;
	system->cpu.opcode[0x4e].cyles = 6;
	// LSR (Absolute,X) 0x5e LEN 3 TIM 7 OK
    system->cpu.opcode[0x5e].fkt = inst_LSR_AX;
	system->cpu.opcode[0x5e].len = 3;
	system->cpu.opcode[0x5e].cyles = 7;
	// AND (Zerro Page) 0x25 LEN 2 TIM 3 OK
	system->cpu.opcode[0x25].fkt = inst_AND_Z;
	system->cpu.opcode[0x25].len = 2;
	system->cpu.opcode[0x25].cyles = 3;
    // AND (Zerro Page,X) 0x35 LEN 2 TIM 4 OK
	system->cpu.opcode[0x35].fkt = inst_AND_ZX;
	system->cpu.opcode[0x35].len = 2;
	system->cpu.opcode[0x35].cyles = 4;
	// STA  (Absolute) 0x8d LEN 3 TIM 4 OK
	system->cpu.opcode[0x8d].fkt = inst_STA_A;
	system->cpu.opcode[0x8d].len = 3;
	system->cpu.opcode[0x8d].cyles = 4;
	// EOR (Immediate) 0x49 LEN 2 TIM 2 OK
	system->cpu.opcode[0x49].fkt = inst_EOR_I;
	system->cpu.opcode[0x49].len = 2;
	system->cpu.opcode[0x49].cyles = 2;
	// EOR (Zeropage) 0x45 LEN 2 TIM 3 OK
	system->cpu.opcode[0x45].fkt = inst_EOR_Z;
	system->cpu.opcode[0x45].len = 2;
	system->cpu.opcode[0x45].cyles = 3;
    // EOR (Indirekt, X) 0x41 LEN 2 TIM 6 OK
	system->cpu.opcode[0x41].fkt = inst_EOR_IX;
	system->cpu.opcode[0x41].len = 2;
	system->cpu.opcode[0x41].cyles = 6;
	// EOR (Zeropage,X) 0x55 LEN 2 TIM 4 OK
	system->cpu.opcode[0x55].fkt = inst_EOR_ZX;
	system->cpu.opcode[0x55].len = 2;
	system->cpu.opcode[0x55].cyles = 4;
	// EOR (Absolute) 0x4d LEN 3 TIM 4 OK
	system->cpu.opcode[0x4d].fkt = inst_EOR_A;
	system->cpu.opcode[0x4d].len = 3;
	system->cpu.opcode[0x4d].cyles = 4;
	// EOR (Absolute,X) 0x5d LEN 3 TIM 4 OK
	system->cpu.opcode[0x5d].fkt = inst_EOR_AX;
	system->cpu.opcode[0x5d].len = 3;
	system->cpu.opcode[0x5d].cyles = 4;//+
	// EOR (Absolute,Y) 0x59 LEN 3 TIM 4 OK
	system->cpu.opcode[0x59].fkt = inst_EOR_AY;
	system->cpu.opcode[0x59].len = 3;
	system->cpu.opcode[0x59].cyles = 4;//+
    // EOR (Indirect,Y) 0x51 LEN 2 TIM 2 OK
	system->cpu.opcode[0x51].fkt = inst_EOR_IY;
	system->cpu.opcode[0x51].len = 2;
	system->cpu.opcode[0x51].cyles = 5;//+
	// INC (Absolute,X) 0xfe LEN 3 TIM 7 OK
	system->cpu.opcode[0xfe].fkt = inst_INC_AX;
	system->cpu.opcode[0xfe].len = 3;
	system->cpu.opcode[0xfe].cyles = 7;
	// ROR (Zero Page) 0x66 LEN 2 TIM 5 OK
	system->cpu.opcode[0x66].fkt = inst_ROR_Z;
	system->cpu.opcode[0x66].len = 2;
	system->cpu.opcode[0x66].cyles = 5;
	// ROR (Zero Page,X) 0x76 LEN 2 TIM 6 OK
	system->cpu.opcode[0x76].fkt = inst_ROR_ZX;
	system->cpu.opcode[0x76].len = 2;
	system->cpu.opcode[0x76].cyles = 6;
	// ROR (Register) 0x6a LEN 1 TIM 2 OK
	system->cpu.opcode[0x6a].fkt = inst_ROR_REG;
	system->cpu.opcode[0x6a].len = 1;
	system->cpu.opcode[0x6a].cyles = 2;
	// ROR (Absolute) 0x6e LEN 3 TIM 6 OK
	system->cpu.opcode[0x6e].fkt = inst_ROR_A;
	system->cpu.opcode[0x6e].len = 3;
	system->cpu.opcode[0x6e].cyles = 6;
	// ROR (Absolute,X) 0x7e LEN 3 TIM 7 OK
	system->cpu.opcode[0x7e].fkt = inst_ROR_AX;
	system->cpu.opcode[0x7e].len = 3;
	system->cpu.opcode[0x7e].cyles = 7;
	// ROL (Accumulator) 0x2a LEN 1 TIM 2 OK
	system->cpu.opcode[0x2a].fkt = inst_ROL;
	system->cpu.opcode[0x2a].len = 1;
	system->cpu.opcode[0x2a].cyles = 2;
	// ROL (ZerroPage) 0x26 LEN 2 TIM 5 OK
	system->cpu.opcode[0x26].fkt = inst_ROL_Z;
	system->cpu.opcode[0x26].len = 2;
	system->cpu.opcode[0x26].cyles = 5;
	// ROL (ZerroPage,X) 0x36 LEN 2 TIM 5 OK
	system->cpu.opcode[0x36].fkt = inst_ROL_ZX;
	system->cpu.opcode[0x36].len = 2;
	system->cpu.opcode[0x36].cyles = 6;
    // ROL (Absolute) 0x2e LEN 3 TIM 6 OK
	system->cpu.opcode[0x2e].fkt = inst_ROL_A;
	system->cpu.opcode[0x2e].len = 3;
	system->cpu.opcode[0x2e].cyles = 6;
	// ROL (Absolute,X) 0x3e LEN 3 TIM 7 OK
	system->cpu.opcode[0x3e].fkt = inst_ROL_AX;
	system->cpu.opcode[0x3e].len = 3;
	system->cpu.opcode[0x3e].cyles = 7;
	// NOP (Implied) 0xea LEN 1 TIM 2 OK
	system->cpu.opcode[0xea].fkt = inst_NOP;
	system->cpu.opcode[0xea].len = 1;
	system->cpu.opcode[0xea].cyles = 2;
	// LDA  (Absolute) 0xad LEN 3 TIM 4 OK
	system->cpu.opcode[0xad].fkt = inst_LDA_A;
	system->cpu.opcode[0xad].len = 3;
	system->cpu.opcode[0xad].cyles = 4;
	// CMP  (Indirect,Y) 0xd1 LEN 2 TIM 5 OK
	system->cpu.opcode[0xd1].fkt = inst_CMP_IY;
	system->cpu.opcode[0xd1].len = 2;
	system->cpu.opcode[0xd1].cyles = 5;//+
	// CMP  (Indirect,X) 0xc1 LEN 2 TIM 6 OK
	system->cpu.opcode[0xc1].fkt = inst_CMP_IX;
	system->cpu.opcode[0xc1].len = 2;
	system->cpu.opcode[0xc1].cyles = 6;
	// PHA  (PusH Accumulator) 0x48 LEN 1 TIM 3 OK
	system->cpu.opcode[0x48].fkt = inst_PHA;
	system->cpu.opcode[0x48].len = 1;
	system->cpu.opcode[0x48].cyles = 3;
	// PLA  (PuLL Accumulator) 0x68 LEN 1 TIM 4 OK
	system->cpu.opcode[0x68].fkt = inst_PLA;
	system->cpu.opcode[0x68].len = 1;
	system->cpu.opcode[0x68].cyles = 4;
	// PLP  (PuLL Processor status) 0x28 LEN 1 TIM 4 OK
	system->cpu.opcode[0x28].fkt = inst_PLP;
	system->cpu.opcode[0x28].len = 1;
	system->cpu.opcode[0x28].cyles = 4;
	// PHP  (PusH Processor status) 0x08 LEN 1 TIM 3 OK
	system->cpu.opcode[0x08].fkt = inst_PHP;
	system->cpu.opcode[0x08].len = 1;
	system->cpu.opcode[0x08].cyles = 3;
	// SBC (Zero Page, X) 0xf5 LEN 2 TIM 4 OK
	system->cpu.opcode[0xf5].fkt = inst_SBC_ZX;
	system->cpu.opcode[0xf5].len = 2;
	system->cpu.opcode[0xf5].cyles = 4;
	// LDY (Absolute,X) 0xbc LEN 3 TIM 4 OK
	system->cpu.opcode[0xbc].fkt = inst_LDY_AX;
	system->cpu.opcode[0xbc].len = 3;
	system->cpu.opcode[0xbc].cyles = 4;//+
	// ORA (Indirect,X) 0x01 LEN 2 TIM 6 OK
	system->cpu.opcode[0x01].fkt = inst_ORA_IX;
	system->cpu.opcode[0x01].len = 2;
	system->cpu.opcode[0x01].cyles = 6;
	// ORA (Indirect,Y) 0x11 LEN 2 TIM 5 OK
	system->cpu.opcode[0x11].fkt = inst_ORA_IY;
	system->cpu.opcode[0x11].len = 2;
	system->cpu.opcode[0x11].cyles = 5;//+
	// BVC (Branch on oVerflow Clear) 0x50 LEN 2 TIM 2 OK
	system->cpu.opcode[0x50].fkt = inst_BVC;
	system->cpu.opcode[0x50].len = 2;
	system->cpu.opcode[0x50].cyles = 2; //+1 branch takes + 1 256 page crossed
	// BVS (Branch on oVerflow Set) 0x70 LEN 2 TIM 2 OK
	system->cpu.opcode[0x70].fkt = inst_BVS;
	system->cpu.opcode[0x70].len = 2;
	system->cpu.opcode[0x70].cyles = 2;//+1 branch takes + 1 256 page crossed
	// CPX (Zero Page) 0xe4  LEN 2 TIM 3 OK
	system->cpu.opcode[0xe4].fkt = inst_CPX_Z;
	system->cpu.opcode[0xe4].len = 2;
	system->cpu.opcode[0xe4].cyles = 3;
	// CMP (Zero Page,X) 0xd5 LEN 2 TIM 4 OK
	system->cpu.opcode[0xd5].fkt = inst_CMP_ZX;
	system->cpu.opcode[0xd5].len = 2;
	system->cpu.opcode[0xd5].cyles = 4;
	// CPX (Absolute) 0xec LEN 3 TIM 4 OK
	system->cpu.opcode[0xec].fkt = inst_CPX_A;
	system->cpu.opcode[0xec].len = 3;
	system->cpu.opcode[0xec].cyles = 4;
    // BIT (Absolute) 0x2c LEN 3 TIM 4 OK
	system->cpu.opcode[0x2c].fkt = inst_BIT_A;
	system->cpu.opcode[0x2c].len = 3;
	system->cpu.opcode[0x2c].cyles = 4;
    // BIT (Zerropage) $24 LEN 2 TIM 3 OK
	system->cpu.opcode[0x24].fkt = inst_BIT_Z;
	system->cpu.opcode[0x24].len = 2;
	system->cpu.opcode[0x24].cyles = 3;
	// SEI 0x78 LEN 1 TIM 2 OK
	system->cpu.opcode[0x78].fkt = inst_SEI;
	system->cpu.opcode[0x78].len = 1;
	system->cpu.opcode[0x78].cyles = 2;
	// CLI 0x58 LEN 1 TIM 2 OK
	system->cpu.opcode[0x58].fkt = inst_CLI;
	system->cpu.opcode[0x58].len = 1;
	system->cpu.opcode[0x58].cyles = 2;
	// CLD 0xd8 LEN 1 TIM 2 OK
	system->cpu.opcode[0xd8].fkt = inst_CLD;
	system->cpu.opcode[0xd8].len = 1;
	system->cpu.opcode[0xd8].cyles = 2;
	// CLV 0xb8 LEN 1 TIM 2 OK
	system->cpu.opcode[0xb8].fkt = inst_CLV;
	system->cpu.opcode[0xb8].len = 1;
	system->cpu.opcode[0xb8].cyles = 2;
	// SED  0xd8 LEN 1 TIM 2 OK
	system->cpu.opcode[0xf8].fkt = inst_SED;
	system->cpu.opcode[0xf8].len = 1;
	system->cpu.opcode[0xf8].cyles = 2;
	// TXS 0x9a LEN 1 TIM 2 OK
	system->cpu.opcode[0x9a].fkt = inst_TXS;
	system->cpu.opcode[0x9a].len = 1;
	system->cpu.opcode[0x9a].cyles = 2;
	// TSX 0xba LEN 1 TIM 2 OK
	system->cpu.opcode[0xba].fkt = inst_TSX;
	system->cpu.opcode[0xba].len = 1;
	system->cpu.opcode[0xba].cyles = 2;
	// INC-SBC 0xff LEN 3 TIM 2
	system->cpu.opcode[0xff].fkt = inst_INC_SBC_A;
	system->cpu.opcode[0xff].len = 1;
	system->cpu.opcode[0xff].cyles = 2;
    // RTI (Implied) LEN 1 TIM 6 OK
	system->cpu.opcode[0x40].fkt = inst_RTI;
	system->cpu.opcode[0x40].len = 1;
	system->cpu.opcode[0x40].cyles = 6;

	fkt_count = 0;
	
	for (i = 0; i < 256; i++)
	{
		if (system->cpu.opcode[i].fkt != inst_unknown)
			fkt_count++;
	}
	
	printf("anzahl OPCODES = %d\n", fkt_count);

	system->cpu.reg.sp = 0xff;
	system->cpu.reg.pc.value = 0x0000;
}

IRAM_ATTR uint32_t tick_6502_system(struct system* system)
{
	uint8_t  opByte;
	uint32_t usedTicks;

	opByte = system->bus.mem[system->cpu.reg.pc.value];
	system->cpu.reg.extra_cyle = 0;
	system->cpu.reg.pc.value = system->cpu.opcode[opByte].fkt(system, &system->bus.mem[system->cpu.reg.pc.value + 1], system->cpu.opcode[opByte].len);
	usedTicks = system->cpu.opcode[opByte].cyles + system->cpu.reg.extra_cyle;
	system->cpu.reg.clocks += usedTicks;
	system->cpu.reg.opcode_long = opByte;
	return usedTicks;
}

IRAM_ATTR void generate_irq_6502_system(struct system* system)
{
	uint8_t cpu_status = 0;
	union word jmpAdr;

	// PC auf dem Stack schreiben
	system->bus.mem[0x100 + system->cpu.reg.sp] = (system->cpu.reg.pc.value & 0xff00) >> 8;
	system->cpu.reg.sp--;
	system->bus.mem[0x100 + system->cpu.reg.sp] = (system->cpu.reg.pc.value & 0xff);
	system->cpu.reg.sp--;

	// Status auf dem Stack schreiben

	//  N | V |	1 | B |	D | I | Z | C
	if (system->cpu.reg.flags.negative)
		cpu_status |= (1 << 7);

	if (system->cpu.reg.flags.overflow)
		cpu_status |= (1 << 6);

	cpu_status |= (1 << 5); // not_used_flag = 1
	cpu_status |= (1 << 4); // break = 1

	if (system->cpu.reg.flags.decimal)
		cpu_status |= (1 << 3);

	if (system->cpu.reg.flags.interrupt)
		cpu_status |= (1 << 2);

	if (system->cpu.reg.flags.zero)
		cpu_status |= (1 << 1);

	if (system->cpu.reg.flags.carry)
		cpu_status |= (1 << 0);

	system->bus.mem[0x100 + system->cpu.reg.sp] = cpu_status;
	system->cpu.reg.sp--;

	system->cpu.reg.flags.interrupt = 1;

	jmpAdr.s.lo = system->bus.mem[0xfffe];
	jmpAdr.s.hi = system->bus.mem[0xffff];

	system->cpu.reg.pc.value = jmpAdr.value;
	system->cpu.reg.clocks += 6; //7?
}

IRAM_ATTR void generate_mni_6502_system(struct system* system)
{
	uint8_t cpu_status = 0;
	union word jmpAdr;

	// PC auf dem Stack schreiben
	system->bus.mem[0x100 + system->cpu.reg.sp] = (system->cpu.reg.pc.value & 0xff00) >> 8;
	system->cpu.reg.sp--;
	system->bus.mem[0x100 + system->cpu.reg.sp] = (system->cpu.reg.pc.value & 0xff);
	system->cpu.reg.sp--;

	// Status auf dem Stack schreiben

	//  N | V |	1 | B |	D | I | Z | C
	if (system->cpu.reg.flags.negative)
		cpu_status |= (1 << 7);

	if (system->cpu.reg.flags.overflow)
		cpu_status |= (1 << 6);

	cpu_status |= (1 << 5); // not_used_flag = 1
	cpu_status |= (1 << 4); // break = 1

	if (system->cpu.reg.flags.decimal)
		cpu_status |= (1 << 3);

	if (system->cpu.reg.flags.interrupt)
		cpu_status |= (1 << 2);

	if (system->cpu.reg.flags.zero)
		cpu_status |= (1 << 1);

	if (system->cpu.reg.flags.carry)
		cpu_status |= (1 << 0);

	system->bus.mem[0x100 + system->cpu.reg.sp] = cpu_status;
	system->cpu.reg.sp--;

	system->cpu.reg.flags.interrupt = 1;

	jmpAdr.s.lo = system->bus.mem[0xfffa];
	jmpAdr.s.hi = system->bus.mem[0xfffb];

	system->cpu.reg.pc.value = jmpAdr.value;
	system->cpu.reg.clocks += 6;
}

IRAM_ATTR uint16_t inst_unknown(struct system *system, uint8_t* opPara, uint32_t len)
{
	return system->cpu.reg.pc.value;
}

IRAM_ATTR uint16_t inst_SEI(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.flags.interrupt = 1;
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_CLI(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.flags.interrupt = 0;
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_CLV(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.flags.overflow = 0;
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_SED(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.flags.decimal = 1;
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_CLD(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.flags.decimal = 0;
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_TSX(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.x = system->cpu.reg.sp;
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_TXS(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.sp = system->cpu.reg.x;
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_LDA_I(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.a = opPara[0];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_LDA_IX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_AND_IX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_AND_IY(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t addr;
	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0]]);
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = ((addr & 0xff) + (system->cpu.reg.y & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif
	
	addr += system->cpu.reg.y;
	system->cpu.reg.a = system->cpu.reg.a & system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_ORA_IX(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t zpAdr;
	uint16_t addr;
	zpAdr = (system->cpu.reg.x + opPara[0]);
	addr = (system->bus.mem[zpAdr + 1] << 8) + (system->bus.mem[zpAdr]);
	system->cpu.reg.a = system->cpu.reg.a | system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_ORA_IY(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = system->bus.mem[opPara[0]];
	adr.s.hi = system->bus.mem[(opPara[0] + 1) & 0xff];
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = (adr.s.lo + (system->cpu.reg.y & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif

	
	system->cpu.reg.a = system->cpu.reg.a | system->bus.mem[adr.value + system->cpu.reg.y];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_LDA_IY(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t addr;
	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0] + 0]);
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = ((addr & 0xff) + (system->cpu.reg.y & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif
	
	addr += system->cpu.reg.y;
	system->cpu.reg.a = system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_CMP_IX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_CMP_IY(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t cmp;
	uint16_t addr;
	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0] + 0]);
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = ((addr & 0xff) + (system->cpu.reg.y & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif

	addr += system->cpu.reg.y;
	
	cmp = system->cpu.reg.a - system->bus.mem[addr];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= system->bus.mem[addr]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_LDX_I(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.x = opPara[0];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_LDA_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.a = system->bus.mem[opPara[0]];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_LDX_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.x = system->bus.mem[opPara[0]];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_LDX_ZY(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.x = system->bus.mem[(opPara[0] + system->cpu.reg.y) & 0xff];

	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_LDY_ZX(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.y = system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];

	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_LDX_A(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.x = system->bus.mem[adr.value];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_LDX_AY(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = (adr.s.lo + (system->cpu.reg.y & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif

	system->cpu.reg.x = system->bus.mem[adr.value + system->cpu.reg.y];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_LDY_A(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.y = system->bus.mem[adr.value];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_STA_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	memory_write(system, opPara[0], system->cpu.reg.a);
	//system->bus.mem[opPara[0]] = system->cpu.reg.a;
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_STX_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	memory_write(system, opPara[0], system->cpu.reg.x);
	//system->bus.mem[opPara[0]] = system->cpu.reg.x;
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_STX_ZY(struct system* system, uint8_t* opPara, uint32_t len)
{
	memory_write(system, (opPara[0] + system->cpu.reg.y) & 0xff, system->cpu.reg.x);
	//system->bus.mem[opPara[0]] = system->cpu.reg.x;
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_STX_A(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	memory_write(system, adr.value, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_STY_A(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	memory_write(system, adr.value, system->cpu.reg.y);
	//system->bus.mem[adr.value] = system->cpu.reg.y;
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_STY_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	memory_write(system, opPara[0], system->cpu.reg.y);

	//system->bus.mem[opPara[0]] = system->cpu.reg.y;
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_STY_ZX(struct system* system, uint8_t* opPara, uint32_t len)
{
	memory_write(system, (opPara[0] + system->cpu.reg.x) & 0xff, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_AND_I(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.a = system->cpu.reg.a & opPara[0];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_AND_A(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.a = (system->cpu.reg.a & system->bus.mem[adr.value]);
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_AND_AX(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = (adr.s.lo + (system->cpu.reg.x & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif

	system->cpu.reg.a = (system->cpu.reg.a & system->bus.mem[adr.value + system->cpu.reg.x]);
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_AND_AY(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = (adr.s.lo + (system->cpu.reg.y & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif

	system->cpu.reg.a = (system->cpu.reg.a & system->bus.mem[adr.value + system->cpu.reg.y]);
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_AND_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.a = system->cpu.reg.a & system->bus.mem[opPara[0]];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_AND_ZX(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.a = system->cpu.reg.a & system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_INC_SBC_A(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_CLC_IM(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.flags.carry = 0;
	// affects no other Flags
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_SEC_IM(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.flags.carry = 1;
	// affects no other Flags
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_ADC_I(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_ADC_IX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_SBC_IX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_ADC_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
	add_op1 = system->cpu.reg.a;
	add_op2 = system->bus.mem[opPara[0]];
	add_result = add_op1 + add_op2 + system->cpu.reg.flags.carry;
	system->cpu.reg.a = add_result;
	// affects C,N,Z,V Flags
	system->cpu.reg.flags.carry = (add_result & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	update_V_Flag(system, add_op1, add_op2, add_result);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_ADC_ZX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_ADC_A(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_SBC_A(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_SBC_AX(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
	union word adr;

	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = (adr.s.lo + (system->cpu.reg.x & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif

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

IRAM_ATTR uint16_t inst_SBC_AY(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
	union word adr;

	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = (adr.s.lo + (system->cpu.reg.y & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif

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

IRAM_ATTR uint16_t inst_ADC_AX(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
	uint16_t addr;

	addr = (opPara[1] << 8) + opPara[0];
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = ((addr & 0xff) + (system->cpu.reg.x & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif

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

IRAM_ATTR uint16_t inst_ADC_AY(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
	uint16_t addr;

	addr = (opPara[1] << 8) + opPara[0];
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = ((addr & 0xff) + (system->cpu.reg.y & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif
	
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

IRAM_ATTR uint16_t inst_CMP_AX(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t addr;
	uint8_t cmp;

	addr = (opPara[1] << 8) + opPara[0];
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = ((addr & 0xff) + (system->cpu.reg.x & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif

	addr += system->cpu.reg.x;
	cmp = system->cpu.reg.a - system->bus.mem[addr];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= system->bus.mem[addr]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_CMP_AY(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t addr;
	uint8_t cmp;

	addr = (opPara[1] << 8) + opPara[0];
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = ((addr & 0xff) + (system->cpu.reg.y & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif

	addr += system->cpu.reg.y;
	cmp = system->cpu.reg.a - system->bus.mem[addr];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= system->bus.mem[addr]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_ADC_IY(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
	uint16_t addr;
	
	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0] + 0]);
	
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = ((addr & 0xff) + (system->cpu.reg.y & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif

	
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

IRAM_ATTR uint16_t inst_SBC_IY(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
	uint16_t addr;

	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0] + 0]);

	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = ((addr & 0xff) + (system->cpu.reg.y & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif
	
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

IRAM_ATTR uint16_t inst_SBC_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t add_op1;
	uint8_t add_op2;
	uint16_t add_result;
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

IRAM_ATTR uint16_t inst_SBC_I(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_LDY_I(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.y = opPara[0];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_STA_IY(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t addr;
	addr = (system->bus.mem[opPara[0] + 1] << 8) + (system->bus.mem[opPara[0] + 0]);
	addr += system->cpu.reg.y;
	
	memory_write(system, addr, system->cpu.reg.a);
	//system->bus.mem[addr] = system->cpu.reg.a;
	// affects no Flags
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_JMP_A(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word jmpAdr;
	jmpAdr.s.lo = opPara[0];
	jmpAdr.s.hi = opPara[1];
	// affects no Flags
	return jmpAdr.value;
}

IRAM_ATTR uint16_t inst_JMP_I(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	union word jmpAdr;

	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];

	jmpAdr.s.lo = system->bus.mem[adr.value];
	jmpAdr.s.hi = system->bus.mem[(adr.value + 1)];
	return jmpAdr.value;
}

IRAM_ATTR uint16_t inst_STA_A(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	
	memory_write(system, adr.value, system->cpu.reg.a);
	//system->bus.mem[adr.value] = system->cpu.reg.a;
	// affects no Flags
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_LDA_A(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.a = system->bus.mem[adr.value];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_STA_IX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_INC_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t tmp;
	tmp = system->bus.mem[opPara[0]];
	tmp++;
	memory_write(system, opPara[0], tmp);
	// affects N,Z Flags
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_INC_ZX(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t tmp;
	tmp = system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];
	tmp++;
	memory_write(system, (opPara[0] + system->cpu.reg.x) & 0xff, tmp);
	// affects N,Z Flags
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_INC_A(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	uint8_t tmp;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	tmp = system->bus.mem[adr.value];
	tmp++;
	memory_write(system, adr.value, tmp);
	// affects N,Z Flags
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_DEC_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t tmp;
	tmp = system->bus.mem[opPara[0]];
	tmp--;
	memory_write(system, opPara[0], tmp);
	// affects N,Z Flags
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_DEC_ZX(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t tmp;
	tmp = system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];
	tmp--;
	memory_write(system, (opPara[0] + system->cpu.reg.x) & 0xff, tmp);
	// affects N,Z Flags
	update_NZ_Flags(system, tmp);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_DEC_A(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_CMP_I(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t cmp;
	cmp = system->cpu.reg.a - opPara[0];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= opPara[0]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_CMP_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t cmp;
	cmp = system->cpu.reg.a - system->bus.mem[opPara[0]];
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= system->bus.mem[opPara[0]]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_CMP_A(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_CPY_A(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_CPY_I(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t cmp;
	cmp = system->cpu.reg.y - opPara[0];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.y >= opPara[0]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_CMX_I(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t cmp;
	cmp = system->cpu.reg.x - opPara[0];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.x >= opPara[0]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_CPX_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t cmp;
	cmp = system->cpu.reg.x - system->bus.mem[opPara[0]];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.x >= system->bus.mem[opPara[0]]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_CMP_ZX(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t cmp;
	cmp = system->cpu.reg.a - system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.a >= system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_CPY_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t cmp;
	cmp = system->cpu.reg.y - system->bus.mem[opPara[0]];
	// affects C,N,Z Flags
	system->cpu.reg.flags.carry = (system->cpu.reg.y >= system->bus.mem[opPara[0]]);
	update_NZ_Flags(system, cmp);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_CPX_A(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_BNE(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t newPc;
	uint16_t oldPC;

	newPc  = system->cpu.reg.pc.value + len;
	
	if(!system->cpu.reg.flags.zero)
	{
		system->cpu.reg.extra_cyle++;
		oldPC = newPc;
		newPc += (int8_t)opPara[0];

		if((oldPC >> 8) != (newPc >> 8))
			system->cpu.reg.extra_cyle++;
	}

	// affects no Flags
	return newPc;
}

IRAM_ATTR uint16_t inst_BCC(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t newPc;
	uint16_t oldPC;
	
	newPc = system->cpu.reg.pc.value + len;
	
	if(!system->cpu.reg.flags.carry)
	{
		system->cpu.reg.extra_cyle++;
		oldPC = newPc;
		newPc += (int8_t)opPara[0];

		if((oldPC >> 8) != (newPc >> 8))
			system->cpu.reg.extra_cyle++;
	}
	
	// affects no Flags
	return newPc;
}

IRAM_ATTR uint16_t inst_BCS(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t newPc;
	uint16_t oldPC;

	newPc = system->cpu.reg.pc.value + len;

	if(system->cpu.reg.flags.carry)
	{
		system->cpu.reg.extra_cyle++;
		oldPC = newPc;
		newPc += (int8_t)opPara[0];

		if((oldPC >> 8) != (newPc >> 8))
			system->cpu.reg.extra_cyle++;
	}

	// affects no Flags
	return newPc;
}

IRAM_ATTR uint16_t inst_BEQ(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t newPc;
	uint16_t oldPC;

	newPc = system->cpu.reg.pc.value + len;
	
	if(system->cpu.reg.flags.zero)
	{
		system->cpu.reg.extra_cyle++;
		oldPC = newPc;

		newPc += (int8_t)opPara[0];

		if((oldPC >> 8) != (newPc >> 8))
			system->cpu.reg.extra_cyle++;
	}
	
	// affects no Flags
	return newPc;
}

IRAM_ATTR uint16_t inst_BVC(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t newPc;
	uint16_t oldPC;

	newPc = system->cpu.reg.pc.value + len;
	
	if(!system->cpu.reg.flags.overflow)
	{
		system->cpu.reg.extra_cyle++;
		oldPC = newPc;
		newPc += (int8_t)opPara[0];

		if((oldPC >> 8) != (newPc >> 8))
			system->cpu.reg.extra_cyle++;
	}

	// affects no Flags
	return newPc;
}

IRAM_ATTR uint16_t inst_BVS(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t newPc;
	uint16_t oldPC;

	newPc = system->cpu.reg.pc.value + len;
	
	if(system->cpu.reg.flags.overflow)
	{
		system->cpu.reg.extra_cyle++;
		oldPC = newPc;
		newPc += (int8_t)opPara[0];
		
		if((oldPC >> 8) != (newPc >> 8))
			system->cpu.reg.extra_cyle++;
	}

	// affects no Flags
	return newPc;
}

IRAM_ATTR uint16_t inst_TAX(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.x = system->cpu.reg.a;
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_TXA(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.a = system->cpu.reg.x;
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_TYA(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.a = system->cpu.reg.y;
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_INY(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.y++;
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_JSR(struct system* system, uint8_t* opPara, uint32_t len)
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
	
	if(pc == 0xafe8)
	{
		system->cpu.reg.anz_PlayJSR++;
	}

	return pc;
}

IRAM_ATTR uint16_t inst_LDY_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.y = system->bus.mem[opPara[0]];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_RTS(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_DEX(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.x--;
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_DEY(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.y--;
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_LDA_AX(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = ((addr & 0xff) + (system->cpu.reg.x & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif

	addr += system->cpu.reg.x;
	system->cpu.reg.a = system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_LDY_AX(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = ((addr & 0xff) + (system->cpu.reg.x & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif

	addr += system->cpu.reg.x;
	system->cpu.reg.y = system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_INC_AX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_DEC_AX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_LDA_AY(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = ((addr & 0xff) + (system->cpu.reg.y & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif

	addr += system->cpu.reg.y;
	system->cpu.reg.a = system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_BPL(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t newPc;
	uint16_t oldPC;

	newPc = system->cpu.reg.pc.value + len;
	
	if(!system->cpu.reg.flags.negative)
	{
		system->cpu.reg.extra_cyle++;
		oldPC = newPc;
		newPc += (int8_t)opPara[0];

		if((oldPC >> 8) != (newPc >> 8))
			system->cpu.reg.extra_cyle++;
	}

	// affects no Flags

	return newPc;
}

IRAM_ATTR uint16_t inst_INX(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.x++;
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.x);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_STA_AX(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.x;
	
	memory_write(system, addr, system->cpu.reg.a);
	//system->bus.mem[addr] = system->cpu.reg.a;
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_STA_AY(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t addr;
	addr = (opPara[1] << 8) + opPara[0];
	addr += system->cpu.reg.y;
	memory_write(system, addr, system->cpu.reg.a);
	//system->bus.mem[addr] = system->cpu.reg.a;
	// no flags change
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_LDA_ZX(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t addr;
	addr = (opPara[0] + system->cpu.reg.x);
	system->cpu.reg.a = system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_SBC_ZX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_STA_ZX(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t addr;
	addr = (opPara[0] + system->cpu.reg.x);
	
	memory_write(system, addr, system->cpu.reg.a);
	//system->bus.mem[addr] = system->cpu.reg.a;
	// affects no Flags
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_ASL(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t shiftVal;
	shiftVal = (system->cpu.reg.a << 1);
	system->cpu.reg.a <<= 1;
	// affects C,N,Z
	system->cpu.reg.flags.carry = (shiftVal & 0x100) != 0;
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_ASL_A(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_ASL_AX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_ASL_Z(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_ASL_ZX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_LSR(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t flag_carry;
	flag_carry = system->cpu.reg.a & 0x01;
	system->cpu.reg.a >>= 1;
	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_carry;
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_LSR_Z(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_LSR_ZX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_LSR_A(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_LSR_AX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_ROR_Z(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_ROR_ZX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_ROR_REG(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t flag_newCarry;
	flag_newCarry = system->cpu.reg.a & 0x01;
	system->cpu.reg.a >>= 1;
	system->cpu.reg.a |= (system->cpu.reg.flags.carry << 7);
	// affects C,N,Z
	system->cpu.reg.flags.carry = flag_newCarry;
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_ROR_A(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_ROR_AX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_ROL(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_ROL_Z(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_ROL_ZX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_ROL_A(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_ROL_AX(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_NOP(struct system* system, uint8_t* opPara, uint32_t len)
{
	// affects no Flags
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_PHA(struct system* system, uint8_t* opPara, uint32_t len)
{
	// auf dem Stack schreiben
	system->bus.mem[0x100 + system->cpu.reg.sp] = system->cpu.reg.a;
	system->cpu.reg.sp--;
	// affects no Flags
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_PLA(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.sp++;
	// vom stack lesen
	system->cpu.reg.a = system->bus.mem[0x100 + system->cpu.reg.sp];
	// affects N,Z
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_PLP(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR uint16_t inst_PHP(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint8_t cpu_status = 0;
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

IRAM_ATTR uint16_t inst_TAY(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.y = system->cpu.reg.a;
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.y);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_BMI(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t newPc;
	uint16_t oldPc;
	
	newPc = system->cpu.reg.pc.value + len;

	if(system->cpu.reg.flags.negative)
	{
		system->cpu.reg.extra_cyle++;
		oldPc = newPc;
		newPc += (int8_t)opPara[0];
		
		if((oldPc >> 8) != (newPc >> 8))
			system->cpu.reg.extra_cyle++;
	}

	// affects no Flags
	return newPc;
}

IRAM_ATTR uint16_t inst_ORA_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.a = system->cpu.reg.a | system->bus.mem[opPara[0]];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_ORA_ZX(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.a = system->cpu.reg.a | system->bus.mem[opPara[0] + system->cpu.reg.x];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_ORA_A(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.a = system->cpu.reg.a | system->bus.mem[adr.value];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_ORA_IM(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.a = system->cpu.reg.a | opPara[0];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_ORA_AX(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = (adr.s.lo + (system->cpu.reg.x & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif
	
	system->cpu.reg.a = system->cpu.reg.a | system->bus.mem[adr.value + system->cpu.reg.x];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_ORA_AY(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = (adr.s.lo + (system->cpu.reg.y & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif
	
	system->cpu.reg.a = system->cpu.reg.a | system->bus.mem[adr.value + system->cpu.reg.y];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_EOR_I(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.a = system->cpu.reg.a ^ opPara[0];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_EOR_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.a = system->cpu.reg.a ^ system->bus.mem[opPara[0]];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_EOR_IX(struct system* system, uint8_t* opPara, uint32_t len)
{
	uint16_t addr;
	addr = (system->bus.mem[opPara[0] + system->cpu.reg.x + 1] << 8) + (system->bus.mem[opPara[0] + system->cpu.reg.x]);
	system->cpu.reg.a = system->cpu.reg.a ^ system->bus.mem[addr];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_EOR_ZX(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.a = system->cpu.reg.a ^ system->bus.mem[(opPara[0] + system->cpu.reg.x) & 0xff];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_EOR_A(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.a = system->cpu.reg.a ^ system->bus.mem[adr.value];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_EOR_AX(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = (adr.s.lo + (system->cpu.reg.x & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif
	
	system->cpu.reg.a = system->cpu.reg.a ^ system->bus.mem[adr.value + system->cpu.reg.x];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_EOR_AY(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = (adr.s.lo + (system->cpu.reg.y & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif

	system->cpu.reg.a = system->cpu.reg.a ^ system->bus.mem[adr.value + system->cpu.reg.y];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_EOR_IY(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = system->bus.mem[opPara[0]];
	adr.s.hi = system->bus.mem[(opPara[0] + 1) & 0xff];
	
	#if 1
	uint32_t extra_cycle_test;
	extra_cycle_test = (adr.s.lo + (system->cpu.reg.y & 0xff));
	if(extra_cycle_test > 255)
		system->cpu.reg.extra_cyle = 1;
	#endif

	system->cpu.reg.a = system->cpu.reg.a ^ system->bus.mem[adr.value + system->cpu.reg.y];
	// affects N,Z Flags
	update_NZ_Flags(system, system->cpu.reg.a);
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_BIT_A(struct system* system, uint8_t* opPara, uint32_t len)
{
	union word adr;
	adr.s.lo = opPara[0];
	adr.s.hi = opPara[1];
	system->cpu.reg.flags.zero = ((system->bus.mem[adr.value] & system->cpu.reg.a) == 0) ? 1: 0;
	system->cpu.reg.flags.negative = ((system->bus.mem[adr.value] & 0x80) != 0) ? 1 : 0;
	system->cpu.reg.flags.overflow = ((system->bus.mem[adr.value] & 0x40) != 0) ? 1 : 0;
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_BIT_Z(struct system* system, uint8_t* opPara, uint32_t len)
{
	system->cpu.reg.flags.zero = ((system->bus.mem[opPara[0]] & system->cpu.reg.a) == 0) ? 1 : 0;
	system->cpu.reg.flags.negative = ((system->bus.mem[opPara[0]] & 0x80) != 0) ? 1 : 0;
	system->cpu.reg.flags.overflow = ((system->bus.mem[opPara[0]] & 0x40) != 0) ? 1 : 0;
	return system->cpu.reg.pc.value + len;
}

IRAM_ATTR uint16_t inst_RTI(struct system* system, uint8_t* opPara, uint32_t len)
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

IRAM_ATTR void update_NZ_Flags(struct system* system, uint8_t value)
{
	system->cpu.reg.flags.negative = (value & 0x80) != 0;
	system->cpu.reg.flags.zero = (value == 0);
}

IRAM_ATTR void update_V_Flag(struct system* system, uint8_t add_op1 , uint8_t add_op2, uint8_t add_result)
{
	// A = Accumulator before the addition
	// v = the value adding to the accumulator
	// s = the sum of the addition (A+v+C)
	if ((add_op1 ^ add_result) & (add_op2 ^ add_result) & 0x80)
		system->cpu.reg.flags.overflow = 1;
	else system->cpu.reg.flags.overflow = 0;
}

#define LOW_BYTE(x)     ((unsigned char)((x)&0xFF))
#define HIGH_BYTE(x)    ((unsigned char)(((x)>>8)&0xFF))

#if 0

// ORGINAL

void memory_write(struct system* system, uint16_t addr, uint8_t value)
{
	system->bus.mem[addr] = value;

	uint8_t adr_byte;

#if 0
	//if(addr == 0xd020)
	// write_emu_u8(s, addr, value);
#endif

if ((addr >= 0xd400) && (addr <= 0xd7ff))
{
	//send(s, (const char*)&addr, 2, 0);
	//send(s, (const char*)&value, 1, 0);

	adr_byte = (addr - 0xd400) & 0x1f;

	//adr_byte = rand() & 0x1f;
	///value = rand() & 0xff;
	//printf("schreibe : %d, %d\n", adr_byte, value);

#if 0

gpio_set_level(18,  rand() & 0x01); // A0
	gpio_set_level(5, rand() & 0x01); // A1
	gpio_set_level(15, rand() & 0x01); // A2
	gpio_set_level(13, rand() & 0x01); // A3
	gpio_set_level(4, rand() & 0x01); // A4
	gpio_set_level(22, rand() & 0x01); // D0
	gpio_set_level(14, rand() & 0x01); // D1
	gpio_set_level(27, rand() & 0x01); // D2
	gpio_set_level(26,rand() & 0x01); // D3
	gpio_set_level(25, rand() & 0x01); // D4
	gpio_set_level(33, rand() & 0x01); // D5
	gpio_set_level(32, rand() & 0x01); // D6
	gpio_set_level(21, rand() & 0x01); // D7
#endif

#if 1
	gpio_set_level(18, ((adr_byte & (1 << 0)) != 0) ? 1: 0); // A0
	gpio_set_level(5, ((adr_byte & (1 << 1)) != 0) ? 1: 0); // A1
	gpio_set_level(15, ((adr_byte & (1 << 2)) != 0) ? 1: 0); // A2
	gpio_set_level(13, ((adr_byte & (1 << 3)) != 0) ? 1: 0); // A3
	gpio_set_level(4, ((adr_byte & (1 << 4)) != 0) ? 1: 0); // A4
	gpio_set_level(22, ((value & (1 << 0)) != 0) ? 1: 0); // D0
	gpio_set_level(14, ((value & (1 << 1)) != 0) ? 1: 0); // D1
	gpio_set_level(27, ((value & (1 << 2)) != 0) ? 1: 0); // D2
	gpio_set_level(26, ((value & (1 << 3)) != 0) ? 1: 0); // D3
	gpio_set_level(25, ((value & (1 << 4)) != 0) ? 1: 0); // D4
	gpio_set_level(33, ((value & (1 << 5)) != 0) ? 1: 0); // D5
	gpio_set_level(32, ((value & (1 << 6)) != 0) ? 1: 0); // D6
	gpio_set_level(21, ((value & (1 << 7)) != 0) ? 1: 0); // D7
#endif

	gpio_set_level(19, 0); // CS
	usleep(2);
	gpio_set_level(19, 1); // CS
	

	#if 0
  gpio_pad_select_gpio(22); // D0
    gpio_pad_select_gpio(23); // CLK
    gpio_pad_select_gpio(21); // D7
    gpio_pad_select_gpio(19); // CS
    gpio_pad_select_gpio(18); // A0
    gpio_pad_select_gpio(5);  // A1
    gpio_pad_select_gpio(15);  // A2
    gpio_pad_select_gpio(2);  // A3
    gpio_pad_select_gpio(4);  // A4
    gpio_pad_select_gpio(14);  // D1
    gpio_pad_select_gpio(27);  // D2
    gpio_pad_select_gpio(26);  // D3
    gpio_pad_select_gpio(25);  // D4
    gpio_pad_select_gpio(33);  // D5
    gpio_pad_select_gpio(32);  // D6

	#endif

#if 0
// GEHT
	system->bus.sendBuf[system->cpu.reg.sendBytes] = LOW_BYTE(addr);
	system->bus.sendBuf[system->cpu.reg.sendBytes+1] = HIGH_BYTE(addr);
	system->bus.sendBuf[system->cpu.reg.sendBytes+2] = value;

	system->cpu.reg.sendBytes+= 3;
	#endif
}
	

//if ((addr >= 0xd400) && (addr <= 0xd7ff))
	//printf("[%d] = %d\n", addr, value);
#if 0
	if ((addr >= 0xd400) && (addr <= 0xd7ff))
		write_emu_u8(s, addr, value);
#endif

}
#endif


#if 0

   #if 0
        /* Blink off (output low) */
	printf("Turning off the LED\n");
        //gpio_set_level(23, 0);
        GPIO.out_w1ts = ((uint32_t)1 << 23);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
	printf("Turning on the LED\n");
        //gpio_set_level(23, 1);
        GPIO.out_w1tc = ((uint32_t)1 << 23);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
        #endif

        #if 0
         GPIO.out_w1tc = 0xffffffff;

         if(flag)
            GPIO.out_w1ts = ((uint32_t)1 << 23);

         flag = !flag;

          vTaskDelay(1000 / portTICK_PERIOD_MS);
          #endif


extern void IRAM_ATTR __digitalWrite(uint8_t pin, uint8_t val)
{
  if(val) {
     if(pin < 32) 
     {
        GPIO.out_w1ts = ((uint32_t)1 << pin);
     } 
     else if(pin < 34) {
        GPIO.out1_w1ts.val = ((uint32_t)1 << (pin - 32));
     }
  } 
  else
  {
     if(pin < 32) 
     {
        GPIO.out_w1tc = ((uint32_t)1 << pin);
     } 
     else if(pin < 34) 
     {
        GPIO.out1_w1tc.val = ((uint32_t)1 << (pin - 32));
     }
  }
}
#endif




// SO GEGTS!
IRAM_ATTR void memory_write(struct system* system, uint16_t addr, uint8_t value)
{
	uint8_t  adr_byte;
	uint32_t write_byte;
	
	// CIA2-LO
	if(addr == 0xdd04)
	{
		system->cia2.timerA_latch &= 0xFF00;
		system->cia2.timerA_latch |= value;
		//system->cia2.timerA_count = system->cia2.timerA_latch;
		return;
	} 

	// CIA2-HI
	if(addr == 0xdd05)
	{
	   system->cia2.timerA_latch &= 0x00FF;
	   system->cia2.timerA_latch |= (value << 8);
	
	  // system->cia2.timerA_count = system->cia2.timerA_latch;
	   return;
	} 

	if(addr == 0xdd0d)
	{
	   if((value & (1 << 7)) != 0)
	   {
		   system->bus.mem[addr] = value;	
	   }
	   else
	   {	
		   while(1)
		   		{};

		    system->bus.mem[addr] &= ~(value & 0x7f);	
	   }

	   return;
	} 

	if((addr >= 0xd400) && (addr <= 0xd7ff))
	{
		adr_byte = (addr - 0xd400) & 0x1f;
		
		GPIO.out_w1tc = (uint32_t) ((1 << 18) | (1 << 5) | (1 << 15) | (1 << 13) | (1 << 4) | (1 << 22) | (1 << 14) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 21));
		GPIO.out1_w1tc.val = (uint32_t) ((1 << 1) | (1 << 0));
		
		write_byte = 0;

		if(((adr_byte & (1 << 0)) != 0))
			write_byte |= (1 << 18);

		if(((adr_byte & (1 << 1)) != 0))
			write_byte |= (1 << 5);

		if(((adr_byte & (1 << 2)) != 0))
			write_byte |= (1 << 15);

		if(((adr_byte & (1 << 3)) != 0))
			write_byte |= (1 << 13);

		if(((adr_byte & (1 << 4)) != 0))
			write_byte |= (1 << 4);

		if(((value & (1 << 0)) != 0))
			write_byte |= (1 << 22);

		if(((value & (1 << 1)) != 0))
			write_byte |= (1 << 14);

		if(((value & (1 << 2)) != 0))
			write_byte |= (1 << 27);

		if(((value & (1 << 3)) != 0))
			write_byte |= (1 << 26);

		if(((value & (1 << 4)) != 0))
			write_byte |= (1 << 25);

		if(((value & (1 << 7)) != 0))
			write_byte |= (1 << 21);

		 GPIO.out_w1ts = write_byte;

		 write_byte = 0;

		if(((value & (1 << 5)) != 0))
			write_byte |= (1 << 1);

		if(((value & (1 << 6)) != 0))
			write_byte |= (1 << 0);

		GPIO.out1_w1ts.val = write_byte;

		GPIO.out_w1tc = (uint32_t) (1 << 19);
		//usleep(1);
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
																					__asm__ __volatile__("nop;"); // Bug workaround (I found this snippet somewhere in this forum)

																				
		
																				
		GPIO.out_w1ts = (uint32_t)(1 << 19);
		return;
	}

	system->bus.mem[addr] = value;	
}



