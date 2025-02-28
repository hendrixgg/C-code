#include <stdio.h>
#include "escape.h"

int main()
{
    const char s[] = "#include <stdio.h>\x0A#include \"escape.h\"\x0A\x0Aint main()\x0A{\x0A    const char s[] = \"\";\x0A    unsigned i;\x0A    // output everything until the first of the empty double quotes.\x0A    for (i = 0; s[i] && s[i+1] && !(i > 0 && s[i-1] == \'\"\' && s[i] == \'\"\'); ++i)\x0A        fputc(s[i], stdout);\x0A    // output the same string but escaped.\x0A    for (unsigned j = 0; s[j]; ++j)\x0A        fputc_escaped(s[j], stdout);\x0A    // output the second double quote and the rest of the string.\x0A    for (; s[i]; ++i)\x0A        fputc(s[i], stdout);\x0A    return 0;\x0A}\x0A";
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
