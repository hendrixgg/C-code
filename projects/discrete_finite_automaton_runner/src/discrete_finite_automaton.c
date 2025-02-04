#include "../include/discrete_finite_automaton.h"
#include <string.h>
#include <assert.h>

const char *discrete_finite_automaton_parse(const discrete_finite_automaton *dfa, const char *const input)
{
    assert(dfa != NULL);
    assert(input != NULL);
    size_t current_state = dfa->start_state;
    const char *p_best = NULL, *p_next = input;
    // does the state make sense?
    // if no, return p_best
    // if yes, check for accept. if accept, update p_best
    // does the next character make sense?
    // if no, return p_best
    // if yes, update current state
    // advance p_next to next character of input.
    while (current_state < dfa->num_states)
    {
        if (dfa->accept_states[current_state])
            p_best = p_next;
        if (*p_next == '\0' || !strchr(dfa->alphabet, *p_next))
            break;
        current_state = dfa->transitions(current_state, *p_next);
        p_next++;
    }
    return p_best;
}

char discrete_finite_automaton_run(const discrete_finite_automaton *dfa, const char *input)
{
    assert(dfa != NULL);
    assert(input != NULL);
    // returns true if the automaton accepted the entire string.
    input = discrete_finite_automaton_parse(dfa, input);
    return input != NULL && *input == '\0';
}