#include <stdio.h>
#include "escape.h"

int main()
{
    int c;
    while ((c = getchar()) != EOF)
        fputc_escaped(c, stdout);
    return 0;
}