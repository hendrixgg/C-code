#include <stdio.h>
#include "escape.h"

int main()
{
    const char s[] = "";
    unsigned i;
    // output everything until the first of the empty double quotes.
    for (i = 0; s[i] && s[i+1] && !(i > 0 && s[i-1] == '"' && s[i] == '"'); ++i)
        fputc(s[i], stdout);
    // output the same string but escaped.
    for (unsigned j = 0; s[j]; ++j)
        fputc_escaped(s[j], stdout);
    // output the second double quote and the rest of the string.
    for (; s[i]; ++i)
        fputc(s[i], stdout);
    return 0;
}
