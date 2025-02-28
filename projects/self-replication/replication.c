#include <stdio.h>
#include "escape.h"

int main()
{
    const char s[] = "#include <stdio.h>\x0A#include \"escape.h\"\x0A\x0Aint main()\x0A{\x0A    const char *s = \"\xFF\";\x0A    for (unsigned i = 0; s[i]; ++i)\x0A        if (s[i] == (char)0xFF)\x0A            for (unsigned j = 0; s[j]; ++j)\x0A                fputc_escaped(s[j], stdout);\x0A        else\x0A            fputc(s[i], stdout);\x0A    return 0;\x0A}\x0A";
    for (unsigned i = 0; s[i]; ++i)
        if (s[i] == (char)0xFF)
            for (unsigned j = 0; s[j]; ++j)
                fputc_escaped(s[j], stdout);
        else
            fputc(s[i], stdout);
    return 0;
}
