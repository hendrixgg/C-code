#include <ctype.h>
#include <assert.h>
#include <string.h>
#include "util.h"

bool starts_with(const char *str, const char *prefix)
{
    assert(str != NULL);
    assert(prefix != NULL);
    return strncmp(str, prefix, strlen(prefix)) == 0;
    // return *prefix == '\0' || *str == *prefix && starts_with(str + 1, prefix + 1);
}

void skip_whitespace(const char **input)
{
    assert(input != NULL);
    assert(*input != NULL);
    while (isspace(**input))
        ++*input;
}
