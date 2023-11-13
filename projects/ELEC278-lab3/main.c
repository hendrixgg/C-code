#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "demo.h"

int main(int argc, char **argv) {
    char data[1024];

    if (argc > 1) {
        // Read test data.
        FILE *file = fopen(argv[1], "r");
        size_t count = fread(data, 1, sizeof(data) - 1, file);
        data[count] = '\0';
        fclose(file);

        bool expected = true;
        if (argc > 2 && strcmp(argv[2], "fail") == 0)
            expected = false;

        // Check brackets.
        assert(check_brackets(data) == expected);
    } else {
        printf("Type some text: ");
        fflush(stdout);
        fgets(data, sizeof(data), stdin);

        if (check_brackets(data))
            printf("correct brackets\n");
        else
            printf("incorrect brackets\n");
    }

    return 0;
}
