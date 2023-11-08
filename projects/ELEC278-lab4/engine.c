#include "engine.h"
#include "util.h"
#include "queue.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

bool run_statements(struct context *ctx, const char **input, struct error *err)
{
    assert(ctx != NULL);
    assert(input != NULL);
    assert(*input != NULL);
    assert(err != NULL);
    // Skip the whitespace at the beginning of the input.
    skip_whitespace(input);
    // Run each statement, if error, return false.
    while (**input != '\0')
    {
        if (!run_statement(ctx, input, err))
            return false;
        // Skip the whitespace at the end of statement.
        skip_whitespace(input);
    }
    // Check if we are at a valid end of a sequence of statements, in which case, do nothing.
    return true;
}

bool bracketed_expression(struct context *ctx, const char **input, struct error *err, int *out)
{
    // Error if the function is not being called.
    if (!starts_with(*input, tokens[TOKEN_CALL_BEGIN]))
    {
        err->pos = *input;
        err->desc = "bracketed_expression: "
                    "TOKEN_CALL_BEGIN not found";
        return false;
    }
    // Increment past the call begin.
    *input += strlen(tokens[TOKEN_CALL_BEGIN]);

    // Evaluate expression. If it evaluation had an error, exit.
    if (!eval_expression(ctx, input, err, out))
        return false;
    // Skip whitespace after expression.
    skip_whitespace(input);

    // Error if function call is not ended.
    if (!starts_with(*input, tokens[TOKEN_CALL_END]))
    {
        err->pos = *input;
        err->desc = "bracketed_expression: "
                    "TOKEN_CALL_END not found";
        return false;
    }
    // Advance past the TOKEN_CALL_END.
    *input += strlen(tokens[TOKEN_CALL_END]);
    return true;
}

bool run_statement(struct context *ctx, const char **input, struct error *err)
{
    assert(ctx != NULL);
    assert(input != NULL);
    assert(*input != NULL);
    assert(err != NULL);
    // Store output from expressions, initially set to 1 so that the while loop iterates once at the start.
    int out = 1;
    // statement classification
    TOKEN statement_token = TOKEN_WHILE * starts_with(*input, tokens[TOKEN_WHILE]) + TOKEN_ASSERT * starts_with(*input, tokens[TOKEN_ASSERT]) + TOKEN_ENQUEUE * starts_with(*input, tokens[TOKEN_ENQUEUE]) + TOKEN_X * starts_with(*input, tokens[TOKEN_X]) + TOKEN_Y * starts_with(*input, tokens[TOKEN_Y]);
    // Should check if the next token is neither whitespace, nor TOKEN_CALL_BEGIN, nor TOKEN_ASSIGN. Using assert for now since this is not going to happen in general.
    *input += strlen(tokens[statement_token]);
    skip_whitespace(input);
    assert(!isalnum(**input));

    // Position after the token.
    const char *after_token = *input;
    switch (statement_token)
    {
    // Task 2: implement enqueue, assert, and assignment statements.
    case TOKEN_ASSERT:
    case TOKEN_ENQUEUE:
        // ENQ or ASSERT
        skip_whitespace(input);

        // Evaluate single function argument.
        if (!bracketed_expression(ctx, input, err, &out))
            return false;

        // Skip whitespace so that input lands on TOKEN_STATEMENT_END.
        skip_whitespace(input);
        // Error if there is no end statement
        if (!starts_with(*input, tokens[TOKEN_STATEMENT_END]))
        {
            err->pos = *input;
            err->desc = "run_statement: "
                        "TOKEN_STATEMENT_END not found";
            return false;
        }
        // Increment past the statement end.
        *input += strlen(tokens[TOKEN_STATEMENT_END]);
        // Execute function.
        switch (statement_token)
        {
        case TOKEN_ASSERT:
            // If expression was false, assign error and exit.
            if (!out)
            {
                err->pos = *input;
                err->desc = "Assertion failed.";
                return false;
            }
            break;
        case TOKEN_ENQUEUE:
            // Enqueue the result from the expression.
            enqueue(&ctx->q, out);
            break;
        default:
            assert(false);
        }
        return true;
        break;
    // Potential variable assignment.
    case TOKEN_X:
    case TOKEN_Y:
        // Error if there is no assignment operator.
        if (!starts_with(*input, tokens[TOKEN_ASSIGN]))
        {
            err->pos = *input;
            err->desc = "run_statement: "
                        "TOKEN_ASSIGN not found";
            return false;
        }
        // Increment past the assignment operator.
        *input += strlen(tokens[TOKEN_ASSIGN]);

        // Skip any whitespace before the expression.
        skip_whitespace(input);
        // Evaluate the value to be assigned. If if failed, exit.
        if (!eval_expression(ctx, input, err, &out))
            return false;

        // Skip any whitespace before TOKEN_STATEMENT_END.
        skip_whitespace(input);
        // Error if there is no end statement
        if (!starts_with(*input, tokens[TOKEN_STATEMENT_END]))
        {
            err->pos = *input;
            err->desc = "run_statement: "
                        "TOKEN_STATEMENT_END not found";
            return false;
        }
        // increment past the statement end.
        *input += strlen(tokens[TOKEN_STATEMENT_END]);

        // Variable assignment
        switch (statement_token)
        {
        case TOKEN_X:
            ctx->x = out;
            break;
        case TOKEN_Y:
            ctx->y = out;
            break;
        default:
            assert(false);
        }

        return true;
    case TOKEN_WHILE:
        // Task 3: implement loops.
        // Loop if the condition is true.
        while (out)
        {
            // Reset input to the point right after TOKEN_WHILE.
            *input = after_token;

            // Evaluate loop condition.
            if (!bracketed_expression(ctx, input, err, &out))
                return false;

            // Skip whitespace after loop condition.
            skip_whitespace(input);
            // If loop does not begin with TOKEN_WHILE_BEGIN, error.
            if (!starts_with(*input, tokens[TOKEN_WHILE_BEGIN]))
            {
                err->pos = *input;
                err->desc = "run_statement: "
                            "WHILE loop missing open bracket.";
                return false;
            }
            // Advance input to the start of the first statement.
            *input += strlen(tokens[TOKEN_WHILE_BEGIN]);
            skip_whitespace(input);
            // Exit the loop if the condition was false.
            if (!out)
            {
                // Find where this while loop ends, accounting for nested loops.
                const char *next_while_end = strstr(*input, tokens[TOKEN_WHILE_END]);
                size_t num_nested_while = 0;
                // Should really be checking for the next TOKEN_WHILE that is followed by either a whitespace, or a TOKEN_CALL_BEGIN.
                for (const char *next_while = strstr(*input, tokens[TOKEN_WHILE]); next_while != NULL && next_while < next_while_end; next_while = strstr(next_while, tokens[TOKEN_WHILE]))
                {
                    num_nested_while++;
                    next_while += strlen(tokens[TOKEN_WHILE]);
                }
                // Skip over (num_nested_while) TOKEN_WHILE_END.
                for (size_t i = 0; next_while_end != NULL && i < num_nested_while; i++)
                {
                    next_while_end += strlen(tokens[TOKEN_WHILE_END]);
                    next_while_end = strstr(next_while_end, tokens[TOKEN_WHILE_END]);
                }
                // Did not find matching TOKEN_WHILE_END.
                if (next_while_end == NULL)
                {
                    err->pos = *input;
                    err->desc = "run_statement: "
                                "WHILE Loop missing closing bracket.";
                    return false;
                }
                // Advance input past TOKEN_WHILE_END
                *input = next_while_end + strlen(tokens[TOKEN_WHILE_END]);
                // Exit the loop.
                break;
            }
            // Would be nice if I could just skip over statements.
            // Run Statements inside the loop. Exit once a statement returns false.
            while (run_statement(ctx, input, err))
            {
                // Skip whitespace between statements.
                skip_whitespace(input);
            };

            // Check if this is the end of the WHILE loop. If not, there was an error with run_statement.
            if (!starts_with(*input, tokens[TOKEN_WHILE_END]))
                return false;
            // Clear the error.
            err->pos = err->desc = NULL;
            // Advance input past TOKEN_WHILE_END
            *input += strlen(tokens[TOKEN_WHILE_END]);
        }
        // Return true if the while loop completed.
        return true;
        break;
    default:
        // Statement not recognized.
        err->pos = *input;
        err->desc = "run_statement: "
                    "Statement not recognized.";
        return false;
    }
}

bool eval_expression(struct context *ctx, const char **input, struct error *err, int *out)
{
    assert(ctx != NULL);
    assert(input != NULL);
    assert(*input != NULL);
    assert(err != NULL);
    assert(out != NULL);
    // True if there is a TOKEN_NOT.
    bool not = false;
    const char *not_pos = *input;
    if (not = starts_with(*input, tokens[TOKEN_NOT]))
        *input += strlen(tokens[TOKEN_NOT]);

    // DEQ
    if (starts_with(*input, tokens[TOKEN_DEQUEUE]))
    {
        // Should not have a "not" for an expression.
        if (not )
        {
            err->pos = not_pos;
            err->desc = "eval_condition: "
                        "Erronius TOKEN_NOT symbol in expression.";
            return false;
        }
        // Store the front element of the queue at out. If failed, exit.
        if (!dequeue(&ctx->q, out))
        {
            err->pos = *input;
            err->desc = "eval_expression->DEQ: "
                        "ctx->q was empty";
            return false;
        }
        *input += strlen(tokens[TOKEN_DEQUEUE]);
    }
    // EMPTY
    else if (starts_with(*input, tokens[TOKEN_EMPTY]))
    {
        *out = queue_empty(&ctx->q);
        *input += strlen(tokens[TOKEN_EMPTY]);
    }
    // constant (integer number) value
    else if ((starts_with(*input, "-") && isdigit(*(*input + 1))) || isdigit(**input))
    {
        // Should not have a "not" for an expression.
        if (not )
        {
            err->pos = not_pos;
            err->desc = "eval_condition: "
                        "Erronius TOKEN_NOT symbol in expression.";
            return false;
        }
        // Parse constant and store it at *out.
        if (!parse_integer(input, err, out))
            return false;
    }
    // variable
    else
    {
        // Should not have a Not for an expression.
        if (not )
        {
            err->pos = not_pos;
            err->desc = "eval_condition: "
                        "Erronius ! symbol in expression";
            return false;
        }
        // True if reading a variable was successful. False otherwise
        bool success = false;
        // variable token
        TOKEN var;
        for (var = TOKEN_X; var <= last_variable_token; var++)
        {
            if (success = starts_with(*input, tokens[var]))
            {
                // Store the value of the variable at *out;
                switch (var)
                {
                case TOKEN_X:
                    *out = ctx->x;
                    break;
                case TOKEN_Y:
                    *out = ctx->y;
                    break;
                default:
                    assert(false);
                }
                // Exit the loop since we have already found the variable.
                break;
            }
        }
        // Not a recognized variable, exit.
        if (!success)
        {
            err->pos = *input;
            err->desc = "eval_expression->variable: "
                        "variable not recognized";
            return false;
        }
        *input += strlen(tokens[var]);
    }
    // Skip to see if the next character is an operator.
    skip_whitespace(input);

    // Check if this is the end of an expression. If true, end the evaluation.
    if (starts_with(*input, tokens[TOKEN_STATEMENT_END]) || starts_with(*input, tokens[TOKEN_CALL_END]))
    {
        // Possibly invert the output.
        *out = not ? !(*out) : (*out);
        return true;
    }

    // There is an operator, check that it is valid.
    TOKEN op = first_binary_operator;
    for (; op <= last_binary_operator; op++)
        if (starts_with(*input, tokens[op]))
            break;

    // Operator is invalid. Exit.
    if (op > last_binary_operator)
    {
        err->pos = *input;
        err->desc = "eval_expression: "
                    "operator is invalid.";
        return false;
    }

    // Skip over the operator and any whitespace.
    *input += strlen(tokens[op]);
    skip_whitespace(input);

    // Used to store the result of the right hand side of the expression.
    int right;
    // Evaluate the right hand side of the expression. If error, exit.
    if (!eval_expression(ctx, input, err, &right))
        return false;

    // Carry out the operation.
    switch (op)
    {
    case TOKEN_ADD:
        *out += right;
        // Should not have a "not" for an expression.
        if (not )
        {
            err->pos = not_pos;
            err->desc = "eval_condition: "
                        "Erronius TOKEN_NOT symbol in expression.";
            return false;
        }
        break;
    case TOKEN_EQUALS:
        *out = (*out == right);
        break;
    case TOKEN_LESS_THAN:
        *out = (*out < right);
        break;
    default:
        assert(false);
    }
    // Possibly invert the output.
    *out = not ? !(*out) : (*out);
    return true;
}

bool eval_condition(struct context *ctx, const char **input, struct error *err, bool *out)
{
    assert(ctx != NULL);
    assert(input != NULL);
    assert(*input != NULL);
    assert(err != NULL);
    assert(out != NULL);
    // True if the condition is to be inverted.
    bool not = false;
    // Task 3: implement not.
    if (not = starts_with(*input, tokens[TOKEN_NOT]))
        *input += strlen(tokens[TOKEN_NOT]);

    int expression_out;
    // Evaluate the expression, possibly following TOKEN_NOT
    if (!eval_expression(ctx, input, err, &expression_out))
        return false;

    // Possibly invert the output.
    *out = not ? (!expression_out) : expression_out;

    return true;
}

bool parse_integer(const char **input, struct error *err, int *out)
{
    // Try to parse an integer at the current position, advancing '*input' to after it.
    const char *before = *input;
    int result = (int)strtol(before, (char **)input, 10);

    // If no characters were used, then the input was invalid.
    if (*input == before)
    {
        err->pos = before;
        err->desc = "expected an integer";
        return false;
    }
    else
    {
        *out = result;
        return true;
    }
}
