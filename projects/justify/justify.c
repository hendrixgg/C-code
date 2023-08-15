#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int line_width = 80;
    /*
    parse the command line arguments,
    change the default line width if it was specified
    */
    char *buffer = malloc(line_width * sizeof(char));
    if (buffer == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        exit(1);
    }

    /*
    while(true) {
        read a word

        if (can't read word) {
            write contents of line buffer to stdout without justification;
            break;
        }
        if (word doesn't fit in line buffer) {
            write contents of line buffer to stdout with justification;
            clear line buffer;
        }
        add word to line buffer;
    }
    see page 386 from book
    */
}