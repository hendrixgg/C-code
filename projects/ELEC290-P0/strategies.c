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

    // random between the other two moves if you lost or tied
    return (enum RPS_CHOICE)((your_choices[round - 1] + (rand() % 2 == 0 ? -1 : 1)) % 3);
}

// Same as strategy #2 in the word document "Exploiting Predictable Irrationality".
enum RPS_CHOICE lose_switch_to_what_would_have_won_win_copy_opponent(
    size_t round,
    size_t game_length,
    enum RPS_CHOICE *your_choices,
    enum RPS_CHOICE *opponent_choices,
    enum GAME_RESULT *results)
{
    // Randomize the first move.
    if (round == 0)
        return (enum RPS_CHOICE)(rand() % 3);

    // switch case over the results of last round.
    switch (results[round - 1])
    {
    // Copy what the opponent did if you won or tied.
    case WIN:
    case TIE:
        return opponent_choices[round - 1];
        break;
    // Switch to what would have won.
    case LOSS:
        return winning_choice_against[opponent_choices[round - 1]];
        break;
    }
}

// // Same as the 4th Strategy in the word document "Utilizing Adaptive Strategies".
enum RPS_CHOICE frequency_exploit(
    size_t round,
    size_t game_length,
    enum RPS_CHOICE *your_choices,
    enum RPS_CHOICE *opponent_choices,
    enum GAME_RESULT *results)
{
    // randomize the first move
    if (round == 0)
        return (enum RPS_CHOICE)(rand() % 3);

    // To be used when there was a loss.
    enum RPS_CHOICE most_frequent_move = ROCK;
    // switch case over the results of last round.
    switch (results[round - 1])
    {
    // If the last round was a win, play what would have beat your move.
    case WIN:
        return winning_choice_against[your_choices[round - 1]];
        break;
    // Play what would beat your opponent in the case of a draw.
    case TIE:
        return losing_choice_against[your_choices[round - 1]];
        break;
    // If the last round was a loss, pick the move that would beat your opponent's most frequently played move.
    case LOSS:
        // calculate opponents most frequent move
        for (size_t i = 0, count_moves[3] = {0, 0, 0}, high_count = 0; i < round; i++)
        {
            count_moves[opponent_choices[i]]++;
            if (count_moves[opponent_choices[i]] > high_count)
            {
                most_frequent_move = opponent_choices[i];
                high_count = count_moves[opponent_choices[i]];
            }
        }
        return winning_choice_against[most_frequent_move];
        break;
    }
}

// This is the strategy that Semyon implemented.
enum RPS_CHOICE semyon_strategy(
    size_t round,
    size_t game_length,
    enum RPS_CHOICE *your_choices,
    enum RPS_CHOICE *opponent_choices,
    enum GAME_RESULT *results)
{
    // randomize the first move
    if (round == 0)
        return (enum RPS_CHOICE)(rand() % 3);

    switch (results[round - 1])
    {
    // If you won/lost:
    case WIN:
    case LOSS:
        // If you played the same symbol twice in a row: randomly pick one of the other moves.
        if (round >= 2 && your_choices[round - 2] == your_choices[round - 1])
            return (enum RPS_CHOICE)((your_choices[round - 1] + (rand() % 2 ? -1 : 1)) % 3);
        // If you played a different symbol the last two moves: 50% chance to stay on the same choice, 50% chance to switch to what would beat your previous choice.
        return rand() % 2 ? your_choices[round - 1] : winning_choice_against[your_choices[round - 1]];
        break;
    case TIE:
        // if there are 2 draws in a row do a random move to break the pattern
        if (round >= 2 && results[round - 2] == results[round - 1])
            return (enum RPS_CHOICE)(rand() % 3);
        // going in the reverse order of RPS
        return losing_choice_against[your_choices[round - 1]];
        break;
    }
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
        return frequency_exploit(round, game_length, your_choices, opponent_choices, results);
        break;
    case 7:
        return semyon_strategy(round, game_length, your_choices, opponent_choices, results);
        break;
    default:
        // This should not happen.
        assert(0);
        break;
    }
}