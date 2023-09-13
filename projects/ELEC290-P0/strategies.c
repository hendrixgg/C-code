#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>

#include "rps_constants.h"

enum RPS_CHOICE always_rock(
    size_t round,
    size_t game_length,
    enum RPS_CHOICE *your_choices,
    enum RPS_CHOICE *opponent_choices,
    enum GAME_RESULT *results)
{
    return ROCK;
}

enum RPS_CHOICE always_paper(
    size_t round,
    size_t game_length,
    enum RPS_CHOICE *your_choices,
    enum RPS_CHOICE *opponent_choices,
    enum GAME_RESULT *results)
{
    return PAPER;
}

enum RPS_CHOICE always_scissors(
    size_t round,
    size_t game_length,
    enum RPS_CHOICE *your_choices,
    enum RPS_CHOICE *opponent_choices,
    enum GAME_RESULT *results)
{
    return SCISSORS;
}

// Same Strategy as "The Art of Deception"
enum RPS_CHOICE random_choice(
    size_t round,
    size_t game_length,
    enum RPS_CHOICE *your_choices,
    enum RPS_CHOICE *opponent_choices,
    enum GAME_RESULT *results)
{
    // TODO: Implement "Draw Lock"

    return (enum RPS_CHOICE)(rand() % 3);
}

// Same as the first strategy in the word document "Win stay, Loss switch".
enum RPS_CHOICE win_stay_lose_switch(
    size_t round,
    size_t game_length,
    enum RPS_CHOICE *your_choices,
    enum RPS_CHOICE *opponent_choices,
    enum GAME_RESULT *results)
{
    // TODO: Implement "Draw Lock"

    // randomize the first move
    if (round == 0)
        return (enum RPS_CHOICE)(rand() % 3);

    // stay if we won last round
    if (results[round - 1] == WIN)
        return your_choices[round - 1];

    // random between the other two if you lost
    int rand_val = rand() % 2;
    int rand_diff = -(rand_val == 0) + (rand_val == 1);
    return (enum RPS_CHOICE)((your_choices[round - 1] + rand_diff) % 3);
}

// Same as strategy #2 in the word document "Exploiting Predictable Irrationality".
enum RPS_CHOICE lose_switch_to_what_would_have_won_win_copy_opponent(
    size_t round,
    size_t game_length,
    enum RPS_CHOICE *your_choices,
    enum RPS_CHOICE *opponent_choices,
    enum GAME_RESULT *results)
{
    // TODO: Implement "Draw Lock".

    // Randomize the first move.
    if (round == 0)
        return (enum RPS_CHOICE)(rand() % 3);

    // Switch to what would have won.
    if (results[round - 1] == LOSS)
        return winning_choice_against[opponent_choices[round - 1]];

    // Copy what the opponent did if you won.
    return opponent_choices[round - 1];
}

// Same as the 4th Strategy in the word document "Utilizing Adaptive Strategies".
enum RPS_CHOICE adaptive_strategy(
    size_t round,
    size_t game_length,
    enum RPS_CHOICE *your_choices,
    enum RPS_CHOICE *opponent_choices,
    enum GAME_RESULT *results)
{
    // TODO: Implement this
    // TODO: Implement "Draw Lock."

    // randomize the first move
    if (round == 0)
        return (enum RPS_CHOICE)(rand() % 3);

    return (enum RPS_CHOICE)(rand() % 3);
}

// Same as the 5th strategy in the word document "Win Situation".
enum RPS_CHOICE win_situation_strategy(
    size_t round,
    size_t game_length,
    enum RPS_CHOICE *your_choices,
    enum RPS_CHOICE *opponent_choices,
    enum GAME_RESULT *results)
{
    // TODO: Implement this
    // TODO: Implement "Draw Lock."

    // randomize the first move
    if (round == 0)
        return (enum RPS_CHOICE)(rand() % 3);

    // leaving this as random for now
    return (enum RPS_CHOICE)(rand() % 3);
}

// implements all the strategies. Accessible by their strategy_num from 0-7.
enum RPS_CHOICE strategy_play(
    size_t strategy_num,
    size_t round,
    size_t game_length,
    enum RPS_CHOICE *your_choices,
    enum RPS_CHOICE *opponent_choices,
    enum GAME_RESULT *results)
{
    switch (strategy_num)
    {
    case 0:
        return always_rock(round, game_length, your_choices, opponent_choices, results);
        break;
    case 1:
        return always_paper(round, game_length, your_choices, opponent_choices, results);
        break;
    case 2:
        return always_scissors(round, game_length, your_choices, opponent_choices, results);
        break;
    case 3:
        return random_choice(round, game_length, your_choices, opponent_choices, results);
        break;
    case 4:
        return win_stay_lose_switch(round, game_length, your_choices, opponent_choices, results);
        break;
    case 5:
        return lose_switch_to_what_would_have_won_win_copy_opponent(round, game_length, your_choices, opponent_choices, results);
        break;
    case 6:
        return adaptive_strategy(round, game_length, your_choices, opponent_choices, results);
        break;
    case 7:
        return win_situation_strategy(round, game_length, your_choices, opponent_choices, results);
        break;
    default:
        // This should not happen.
        assert(0);
        break;
    }
}