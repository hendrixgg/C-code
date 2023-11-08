#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "engine.h"

void print_err(const char *input, struct error err)
{
    const char *start = input;
    const char *cur = input;
    while (cur < err.pos)
    {
        if (*cur == '\n' || *cur == '\r')
            start = cur + 1;
        cur++;
    }
    const char *end = err.pos;
    while (*end && *end != '\n' && *end != '\r')
        end++;

    size_t line_size = end - start;
    char line[line_size + 1];
    memcpy(line, start, line_size);
    line[line_size] = '\0';
    fprintf(stderr, "%s\n", line);

    for (size_t i = 0; i < err.pos - start; i++)
        putc(' ', stderr);
    fputs("^\n", stderr);
    fprintf(stderr, "Error: %s\n", err.desc);
}

int main(int argc, char **argv)
{
    struct context ctx = {
        .x = 0,
        .y = 0,
    };
    init_queue(&ctx.q);

    char data[65536];

    const char *input = data;
    struct error err = {
        .pos = NULL,
        .desc = NULL,
        .test_output = fopen("../test_output.txt", "w"),
        .test_input = fopen("../test-cases/case4.txt", "r"),
    };

    if (argc > 1)
    // if (false)
    {
        // Read test data.
        FILE *file = fopen(argv[1], "r");
        size_t count = fread(data, 1, sizeof(data) - 1, file);
        data[count] = '\0';
        fclose(file);

        bool expected = true;
        if (argc > 2 && strcmp(argv[2], "fail") == 0)
            expected = false;

        bool result = run_statements(&ctx, &input, &err);
        if (!result)
            print_err(data, err);
        assert(result == expected);
        if (expected)
            assert(*input == '\0');
    }
    else
    {
        // printf("Type some text: ");
        // fflush(stdout);
        // fgets(data, sizeof(data), stdin);
        size_t count = fread(data, sizeof(*data), sizeof(data) - 1, err.test_input);
        data[count] = '\0';
        fclose(err.test_input);
        fprintf(err.test_output, "--- test started ---\n");

        if (run_statements(&ctx, &input, &err))
            printf("success\nx = %d\ny = %d\n", ctx.x, ctx.y);
        else
        {
            printf("failure\n");
            print_err(data, err);
            printf("x = %d\ny = %d\n", ctx.x, ctx.y);
        }
    }
    fprintf(err.test_output, "--- test ended ---\n");

    fclose(err.test_output);
    return 0;
}
