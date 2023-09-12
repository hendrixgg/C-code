#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>

#include "rps_constants.h"

enum RPS_CHOICE rock_always(
    size_t round,
    enum RPS_CHOICE your_choices[GAME_LENGTH],
    enum RPS_CHOICE opponent_choices[GAME_LENGTH],
    enum GAME_RESULT results[GAME_LENGTH])
{
    return ROCK;
}

enum RPS_CHOICE paper_always(
    size_t round,
    enum RPS_CHOICE your_choices[GAME_LENGTH],
    enum RPS_CHOICE opponent_choices[GAME_LENGTH],
    enum GAME_RESULT results[GAME_LENGTH])
{
    return PAPER;
}

enum RPS_CHOICE scissors_always(
    size_t round,
    enum RPS_CHOICE your_choices[GAME_LENGTH],
    enum RPS_CHOICE opponent_choices[GAME_LENGTH],
    enum GAME_RESULT results[GAME_LENGTH])
{
    return SCISSORS;
}

// Same Strategy as "The Art of Deception"
enum RPS_CHOICE random_choice(
    size_t round,
    enum RPS_CHOICE your_choices[GAME_LENGTH],
    enum RPS_CHOICE opponent_choices[GAME_LENGTH],
    enum GAME_RESULT results[GAME_LENGTH])
{
    // TODO: Implement "Draw Lock"

    return (enum RPS_CHOICE)(rand() % 3);
}

// Same as the first strategy in the word document "Win stay, Loss switch".
enum RPS_CHOICE win_stay_lose_switch(
    size_t round,
    enum RPS_CHOICE your_choices[GAME_LENGTH],
    enum RPS_CHOICE opponent_choices[GAME_LENGTH],
    enum GAME_RESULT results[GAME_LENGTH])
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
    enum RPS_CHOICE your_choices[GAME_LENGTH],
    enum RPS_CHOICE opponent_choices[GAME_LENGTH],
    enum GAME_RESULT results[GAME_LENGTH])
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
    enum RPS_CHOICE your_choices[GAME_LENGTH],
    enum RPS_CHOICE opponent_choices[GAME_LENGTH],
    enum GAME_RESULT results[GAME_LENGTH])
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
    enum RPS_CHOICE your_choices[GAME_LENGTH],
    enum RPS_CHOICE opponent_choices[GAME_LENGTH],
    enum GAME_RESULT results[GAME_LENGTH])
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
    enum RPS_CHOICE your_choices[GAME_LENGTH],
    enum RPS_CHOICE opponent_choices[GAME_LENGTH],
    enum GAME_RESULT results[GAME_LENGTH])
{
    switch (strategy_num)
    {
    case 0:
        return rock_always(round, your_choices, opponent_choices, results);
        break;
    case 1:
        return paper_always(round, your_choices, opponent_choices, results);
        break;
    case 2:
        return scissors_always(round, your_choices, opponent_choices, results);
        break;
    case 3:
        return random_choice(round, your_choices, opponent_choices, results);
        break;
    case 4:
        return win_stay_lose_switch(round, your_choices, opponent_choices, results);
        break;
    case 5:
        return lose_switch_to_what_would_have_won_win_copy_opponent(round, your_choices, opponent_choices, results);
        break;
    case 6:
        return adaptive_strategy(round, your_choices, opponent_choices, results);
        break;
    case 7:
        return win_situation_strategy(round, your_choices, opponent_choices, results);
        break;
    default:
        // This should not happen.
        assert(0);
        break;
    }
}