#include <stddef.h>

/**
 * `num_states`: The number of states in the discrete finite automaton. Valid states are in the range [0, `num_states`).
 * `alphabet`: A null-terminated string containing all the characters that the automaton can accept (the automaton cannot accept the null character `'\0'` as that character is reserved to mark the end of the input sequence).
 * `transitions`: A function that takes a state and an input character and returns the next state. If the state or input character is invalid, the function should return a state such that `state >= num_states`.
 * `start_state`: The state that the automaton starts in. (0 <= `start_state` < `num_states`)
 * `accept_states`: An array of bytes where the accepting states are marked with a non-zero byte. The array should be of length `num_states`. If (accept_states[i] != 0), then state i is an accepting state, if (accept_states[i] == 0), then state i is a reject state.
 */
typedef struct _discrete_finite_automaton
{
    size_t num_states;
    const char *alphabet;
    size_t (*transitions)(size_t, char);
    size_t start_state;
    const char *accept_states;
} discrete_finite_automaton;

/**
 * Parses an input string using a discrete finite automaton.
 * @param dfa The discrete finite automata to parse the input string with.
 * @param input The input string to parse.
 * @return A pointer to one character after the longest prefix of the input string that the automata accepts. If the automata rejects all prefixes of the input string, including the empty string, the function returns a null pointer.
 */
const char *discrete_finite_automaton_parse(const discrete_finite_automaton *dfa, const char *const input);

/**
 * Runs a discrete finite automata on an input string. The automata will
 * automatically reject the input if it encounters a character not in
 * the alphabet or if the transition function returns a state such that
 * `state >= num_states`. The automata will consider the terminating null
 * character (`'\0'`) as the end of the input string.
 * @param dfa The discrete finite automata to run.
 * @param input The input string to run the automata on.
 * @return non-zero if the discrete_finite_automata accepts the string. zero if the finite automata rejects the string.
 */
char discrete_finite_automaton_run(const discrete_finite_automaton *dfa, const char *input);