#include <stdio.h>
#include "escape.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
    {
        perror("fopen");
        return 1;
    }
    int c_prev = 0, c;
    // Output everything until the first empty double quotes.
    while ((c = fgetc(f)) != EOF && !(c_prev == '"' && c == '"'))
    {
        fputc(c, stdout);
        c_prev = c;
    }
    // Output everything from standard input in between the quotes.
    while((c = fgetc(stdin)) != EOF)
        fputc(c, stdout);
    // Output the second double quote.
    fputc(c_prev, stdout);
    // Output the rest of stuff from the file.
    while ((c = fgetc(f)) != EOF)
        fputc(c, stdout);
    fclose(f);
    return 0;
}