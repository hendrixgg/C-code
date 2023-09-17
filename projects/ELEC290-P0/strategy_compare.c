#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "rps_constants.h"

#include "strategies.h"

#define NUM_GAMES 10000000ULL
#define GAME_LENGTH 7ULL

enum GAME_RESULT get_result(enum RPS_CHOICE your_choice, enum RPS_CHOICE opponent_choice)
{
    return (enum GAME_RESULT)(-(your_choice == opponent_choice) + (your_choice == winning_choice_against[opponent_choice]));
}

// results = {player1_win%, player2_win%, tie%}
void run_simulation(size_t num_games, size_t num_rounds, size_t strategy_player1, size_t strategy_player2, float results[3])
{
    srand(time(NULL));
    enum RPS_CHOICE player1[num_rounds], player2[num_rounds];
    enum GAME_RESULT player1_results[num_rounds], player2_results[num_rounds];
    size_t player1_wins = 0, player2_wins = 0, num_ties = 0;
    for (size_t game_num = 0; game_num < num_games; game_num++)
    {
        // number of wins this round
        size_t player1_round_wins = 0, player2_round_wins = 0;
        for (size_t round_num = 0; round_num < num_rounds; round_num++)
        {
            // get the moves
            player1[round_num] = strategy_play(strategy_player1, round_num, num_rounds, player1, player2, player1_results);
            player2[round_num] = strategy_play(strategy_player2, round_num, num_rounds, player2, player1, player2_results);

            // calculate win/loss
            player1_results[round_num] = get_result(player1[round_num], player2[round_num]);
            player2_results[round_num] = get_result(player2[round_num], player1[round_num]);

            // increment win/loss for the game
            player1_round_wins += player1_results[round_num] == WIN;
            player2_round_wins += player2_results[round_num] == WIN;
        }

        // increment win/loss for the total simulation
        player1_wins += player1_round_wins > player2_round_wins;
        player2_wins += player2_round_wins > player1_round_wins;
        num_ties += player1_round_wins == player2_round_wins;
    }

    // calculate win % and tie %
    results[0] = player1_wins * 100.0 / num_games;
    results[1] = player2_wins * 100.0 / num_games;
    results[2] = num_ties * 100.0 / num_games;
}

void simulate_all_strategies(size_t num_strategies, size_t num_games, size_t num_rounds_per_game)
{
    float sim_results[num_strategies][num_strategies][3];
    // run the games
    for (size_t i = 0; i < num_strategies; i++)
    {
        for (size_t j = 0; j < num_strategies; j++)
        {
            run_simulation(num_games, num_rounds_per_game, i, j, sim_results[i][j]);
        }
    }

    // print the results
    FILE *results_file = fopen("results.csv", "w");
    for (size_t i = 0; i < num_strategies; i++)
    {
        for (size_t j = 0; j < num_strategies; j++)
        {
            // fprintf(results_file, "\"%.0f, %.0f, %.0f\",", sim_results[i][j][0], sim_results[i][j][1], sim_results[i][j][2]);
            fprintf(results_file, "\"%.0f, %.0f\",", sim_results[i][j][0], sim_results[i][j][2]);
            // printf("\"%.0f, %.0f, %.0f\",", sim_results[i][j][0], sim_results[i][j][1], sim_results[i][j][2]);
            // printf("\"%.0f, %.0f\",", sim_results[i][j][0], sim_results[i][j][2]);
        }
        fprintf(results_file, "\n");
        // printf("\n");
    }
    fclose(results_file);
}

int main()
{

    (void)simulate_all_strategies(NUM_STRATEGIES, NUM_GAMES, GAME_LENGTH);
    return 0;
}