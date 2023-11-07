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

    // Check if we are at a valid end of a sequence of statements, in which case, do nothing.
    if (**input == '\0' || **input == '}')
        return true;

    // Otherwise, run a single statement and then the rest of the statements.
    return run_statement(ctx, input, err) && run_statements(ctx, input, err);
}

bool run_statement(struct context *ctx, const char **input, struct error *err)
{
    assert(ctx != NULL);
    assert(input != NULL);
    assert(*input != NULL);
    assert(err != NULL);
    // TODO: Task 2: implement enqueue, assert, and assignment statements.
    int out;
    // ENQ
    if (starts_with(*input, tokens[TOKEN_ENQUEUE]))
    {
        // Increment input just past the token.
        *input += strlen(tokens[TOKEN_ENQUEUE]);
        skip_whitespace(input);

        // Error if the function is not being called.
        if (!starts_with(*input, tokens[TOKEN_CALL_BEGIN]))
        {
            err->pos = *input;
            err->desc = "run_statement->ENQUEUE: "
                        "TOKEN_CALL_BEGIN not found";
            return false;
        }
        // Increment past the call begin.
        *input += strlen(tokens[TOKEN_CALL_BEGIN]);
        skip_whitespace(input);

        // ENQ specific
        // Evaluate the argument. If it failed, exit.
        if (!eval_expression(ctx, input, err, &out))
            return false;
        skip_whitespace(input);

        // Error if function call is not ended.
        if (!starts_with(*input, tokens[TOKEN_CALL_END]))
        {
            err->pos = *input;
            err->desc = "run_statement->ENQUEUE: "
                        "TOKEN_CALL_END not found";
            return false;
        }
        // Increment past the close bracket.
        *input += strlen(tokens[TOKEN_CALL_END]);
        skip_whitespace(input);
        // Error if there is no end statement
        if (!starts_with(*input, tokens[TOKEN_STATEMENT_END]))
        {
            err->pos = *input;
            err->desc = "run_statement->ENQUEUE: "
                        "TOKEN_STATEMENT_END not found";
            return false;
        }
        // increment past the statement end.
        *input += strlen(tokens[TOKEN_STATEMENT_END]);
        skip_whitespace(input);

        // Enqueue the result from the expression.
        enqueue(&ctx->q, out);

        return true;
    }
    // ASSERT
    if (starts_with(*input, tokens[TOKEN_ASSERT]))
    {
        // Increment input just past the token.
        *input += strlen(tokens[TOKEN_ASSERT]);
        skip_whitespace(input);

        // Error if the function is not being called.
        if (!starts_with(*input, tokens[TOKEN_CALL_BEGIN]))
        {
            err->pos = *input;
            err->desc = "run_statement->ASSERT: "
                        "TOKEN_CALL_BEGIN not found";
            return false;
        }
        // Increment past the call begin.
        *input += strlen(tokens[TOKEN_CALL_BEGIN]);

        // Evaluate the argument. If it failed, exit.
        if (!eval_expression(ctx, input, err, &out))
            return false;
        skip_whitespace(input);

        // Error if function call is not ended.
        if (!starts_with(*input, tokens[TOKEN_CALL_END]))
        {
            err->pos = *input;
            err->desc = "run_statement->ASSERT: "
                        "TOKEN_CALL_END not found";
            return false;
        }
        // Increment past the close bracket.
        *input += strlen(tokens[TOKEN_CALL_END]);
        skip_whitespace(input);
        // Error if there is no end statement
        if (!starts_with(*input, tokens[TOKEN_STATEMENT_END]))
        {
            err->pos = *input;
            err->desc = "run_statement->ASSERT: "
                        "TOKEN_STATEMENT_END not found";
            return false;
        }
        // increment past the statement end.
        *input += strlen(tokens[TOKEN_STATEMENT_END]);

        // If expression was true, so return true.
        if (out)
            return true;
        // If expression was false, assign error and exit.
        err->pos = *input;
        err->desc = "run_statement->ASSERT: "
                    "Assertion failed";
        return false;
    }
    // Variable Assignment
    for (TOKEN i = 0; i < num_variable_tokens; i++)
    {
        // If there is a matching variable.
        if (starts_with(*input, tokens[i]))
        {
            // Advance input past the variable token.
            *input += strlen(tokens[i]);
            skip_whitespace(input);

            // Error if there is no assignment operator.
            if (!starts_with(*input, tokens[TOKEN_ASSIGN]))
            {
                err->pos = *input;
                err->desc = "run_statement->Variable Assignment: "
                            "TOKEN_ASSIGN not found";
                return false;
            }
            // Increment past the assignment operator.
            *input += strlen(tokens[TOKEN_ASSIGN]);
            skip_whitespace(input);

            // Evaluate the value to be assigned. If if failed, exit.
            if (!eval_expression(ctx, input, err, &out))
                return false;
            skip_whitespace(input);

            // Error if there is no end statement
            if (!starts_with(*input, tokens[TOKEN_STATEMENT_END]))
            {
                err->pos = *input;
                err->desc = "run_statement->ASSERT: "
                            "TOKEN_STATEMENT_END not found";
                return false;
            }
            // increment past the statement end.
            *input += strlen(tokens[TOKEN_STATEMENT_END]);
            skip_whitespace(input);
            // Variable assignment
            switch (i)
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
        }
    }

    // TODO: Task 3: implement loops.
    // if (starts_with(*input, tokens[TOKEN_WHILE]))
    // {
    // }

    // Statement not recognized.
    return false;
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
    // TODO: Task 3: implement not.
    if (not = starts_with(*input, tokens[TOKEN_NOT]))
        *input += strlen(tokens[TOKEN_NOT]);

    // DEQ
    if (starts_with(*input, tokens[TOKEN_DEQUEUE]))
    {
        // Store the front element of the queue at *out. If failed, exit.
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
        // Parse constant and store it at *out.
        if (!parse_integer(input, err, out))
            return false;
    }
    // variable
    else
    {
        // True if reading a variable was successful. False otherwise
        bool success = false;
        // variable token
        TOKEN var;
        for (var = TOKEN_X; var < num_variable_tokens; var++)
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
        return true;

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
