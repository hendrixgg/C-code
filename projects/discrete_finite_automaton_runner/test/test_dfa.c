#include <stdio.h>
#include "../include/discrete_finite_automaton.h"

/*
fa_description = {
    "states": {"q0", "q1", "q2"} ,
    "alphabet": {"0", "1"} ,
    "transitions": {
        ("q0", "0"): "q1",
        ("q0", "1"): "q0",
        ("q1", "0"): "q2",
        ("q1", "1"): "q0",
        ("q2", "0"): "q2",
        ("q2", "1"): "q2",
    },
    "start_state": "q0" ,
    "accept_states": {"q2"},
}
input_strings = ["0", "01", "001", "0001", "111", "10", "000"]
*/

#define NUM_STATES 3
#define LENGTH_ALPHABET 2
size_t dfa_transition_function(size_t state, char input)
{
    static const char transition[NUM_STATES][LENGTH_ALPHABET] = {
        {1, 0},
        {2, 0},
        {2, 2}};
    if (state < 3 && '0' <= input && input <= '1')
        return transition[state][input - '0'];
    return 3; // invalid state/input
}

const discrete_finite_automaton dfa = {
    .num_states = NUM_STATES,
    .alphabet = "01",
    .transitions = dfa_transition_function,
    .start_state = 0,
    .accept_states = (char[]){0, 0, 1},
};

int main()
{
    const char *input_strings[] = {"0", "01", "001", "0001", "111", "10", "000"}, **test_string;
    for (test_string = input_strings;
         test_string < input_strings + sizeof(input_strings) / sizeof(input_strings[0]);
         test_string++)
        printf("Input %s -> Accepted: %s\n", *test_string, discrete_finite_automaton_run(&dfa, *test_string) ? "True" : "False");
    return 0;
}