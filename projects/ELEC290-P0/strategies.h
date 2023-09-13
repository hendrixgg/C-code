#pragma once

#include "rps_constants.h"

#define NUM_STRATEGIES 6ULL

enum RPS_CHOICE strategy_play(
    size_t strat_num,
    size_t round,
    size_t game_length,
    enum RPS_CHOICE *your_choices,
    enum RPS_CHOICE *opponent_choices,
    enum GAME_RESULT *results);
