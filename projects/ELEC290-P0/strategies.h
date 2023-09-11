#ifndef STRATEGIES
#define STRATEGIES

#include <inttypes.h>

#include "rps_constants.h"

#define NUM_STRATEGIES 6ULL

enum RPS_CHOICE strategy_play(
    size_t strat_num,
    size_t round,
    enum RPS_CHOICE your_choices[GAME_LENGTH],
    enum RPS_CHOICE opponent_choices[GAME_LENGTH],
    enum GAME_RESULT results[GAME_LENGTH]);

#endif