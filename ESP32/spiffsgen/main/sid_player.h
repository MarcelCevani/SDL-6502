#pragma once

#include <stdint.h>
#include "core_6502.h"

struct sid_player
{
	uint8_t currentSongNr;
	uint8_t vBlankInterruptTime; // 50/60 Hz
	uint16_t playerOffset;
	uint32_t flag_emulate_frame;

	struct system system_6502;
};

void init_sid_player(struct sid_player* system);
uint8_t load_sid_file(struct sid_player* player, char* pFileName);
uint8_t load_sid_from_memory(struct sid_player* player, uint32_t memorySize, uint8_t * sid_memory);
uint8_t init_sid_tune(struct sid_player* player, uint8_t tune_nr);
void play_sid_tune(struct sid_player* player);