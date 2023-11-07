#include <ctype.h>
#include "util.h"

bool starts_with(const char *str, const char *prefix) {
    return *prefix == '\0' || *str == *prefix && starts_with(str + 1, prefix + 1);
}

void skip_whitespace(const char **input) {
    while (isspace(**input))
        ++*input;
}
