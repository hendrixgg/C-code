#ifndef LAB4_ENGINE_H
#define LAB4_ENGINE_H

#include <stdbool.h>

// added
#include <stdio.h>
// added

#include "queue.h"

struct context
{
    // Queue of elements.
    struct queue q;
    // Temporary variables.
    int x, y;
};

// Represents an error that occurred while parsing or evaluating the input.
struct error
{
    // The position in the input string where the error occurred.
    const char *pos;
    // A description of the error; this must be a string literal, not a dynamically allocated string.
    const char *desc;
    // Files for output while debugging.
    FILE *test_output, *test_input;
};

// Stores the different tokens that can be found in a script.
static const char *tokens[] = {
    "x",
    "y",
    "DEQ",
    "EMPTY",
    "ASSERT",
    "ENQ",
    "WHILE",
    "=",
    "+",
    "=",
    "<",
    "!",
    ";",
    "(",
    ")",
    "{",
    "}",
    " =(",
};

// Enum for accessing tokens in the above array.
typedef enum _TOKEN
{
    TOKEN_X = 0,
    TOKEN_Y = 1,
    TOKEN_DEQUEUE = 2,
    TOKEN_EMPTY = 3,
    TOKEN_ASSERT = 4,
    TOKEN_ENQUEUE = 5,
    TOKEN_WHILE = 6,
    TOKEN_ASSIGN = 7,
    TOKEN_ADD = 8,
    TOKEN_EQUALS = 9,
    TOKEN_LESS_THAN = 10,
    TOKEN_NOT = 11,
    TOKEN_STATEMENT_END = 12,
    TOKEN_CALL_BEGIN = 13,
    TOKEN_CALL_END = 14,
    TOKEN_WHILE_BEGIN = 15,
    TOKEN_WHILE_END = 16,
    TOKEN_STATEMENT_SPLIT = 17,
} TOKEN;

// Number of variable tokens.
static const size_t num_variable_tokens = 2;
// Bounds for binary operator tokens. binary operator tokens are located sequentially in the tokens array.
static const TOKEN first_binary_operator = TOKEN_ADD, last_binary_operator = TOKEN_LESS_THAN;

// Runs a sequence of statements, advancing '*input' to the end of the sequence.
// Returns false and sets '*err' if the input was invalid.
bool run_statements(struct context *ctx, const char **input, struct error *err);

// Runs a single statement, advancing '*input' to the end of the statement.
// Returns false and sets '*err' if the input was invalid.
bool run_statement(struct context *ctx, const char **input, struct error *err);

// Evaluates an expression, advancing '*input' to the end of the expression and storing the result in '*out'.
// Returns false and sets '*err' if the input was invalid.
bool eval_expression(struct context *ctx, const char **input, struct error *err, int *out);

// Evaluates a condition, advancing '*input' to the end of the condition and storing the result in '*out'.
// Returns false and sets '*err' if the input was invalid.
bool eval_condition(struct context *ctx, const char **input, struct error *err, bool *out);

// Tries to parse an integer.
bool parse_integer(const char **input, struct error *err, int *out);

#endif // LAB4_ENGINE_H
