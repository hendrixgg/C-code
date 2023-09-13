#ifndef RPS_CONSTANTS
#define RPS_CONSTANTS

enum RPS_CHOICE
{
    ROCK = 0,
    PAPER = 1,
    SCISSORS = 2
};

enum GAME_RESULT
{
    TIE = -1,
    LOSS = 0,
    WIN = 1
};

static const char *RPS_CHOICE_to_str[3] = {"ROCK", "PAPER", "SCISSORS"};

/*
  Returns the RPS choice that wins against choice.
  e.g. winningChoiceAgainst(ROCK) = PAPER
*/
static const enum RPS_CHOICE winning_choice_against[3] = {PAPER, SCISSORS, ROCK};

/*
  Returns the RPS choice that loses against choice.
  e.g. winningChoiceAgainst(ROCK) = PAPER
*/
static const enum RPS_CHOICE losing_choice_against[3] = {SCISSORS, ROCK, PAPER};

#endif