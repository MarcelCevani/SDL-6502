#pragma once

#include <stdint.h>
#include "core_6502.h"

struct sub_system
{
	uint16_t vic_irq_rasterline;
	uint8_t  vic_int_ctrl_register;
};

struct sid_player
{
	uint8_t currentSongNr;
	uint8_t vBlankInterruptTime; // 50/60 Hz
	uint16_t playerOffset;
	struct system system_6502;
	struct sub_system sub_system;
};

extern "C" void init_sid_player(struct sid_player* system);
extern "C" uint8_t load_sid_file(struct sid_player* player, char* pFileName);
extern "C" uint8_t load_sid_from_memory(struct sid_player* player, uint32_t memorySize, uint8_t * sid_memory);
extern "C" uint8_t init_sid_tune(struct sid_player* player, uint8_t tune_nr);
extern "C" uint8_t play_sid_tune(struct sid_player* player, uint32_t max_cyles);