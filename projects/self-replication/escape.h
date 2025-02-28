#ifndef ESCAPE_H
#define ESCAPE_H
#include <stdio.h>

typedef enum _ESCAPE { ESCAPE_NONE, ESCAPE_NORMAL, ESCAPE_HEX } ESCAPE;

/**
 * @brief Determine the escape type of a character.
 * Escape Normal: \a, \b, \f, \n, \r, \t, \v, \\, \', \"
 * Escape Hex: Any character that is not Escape Normal and outside the range 32-126.
 * Escape None: Any character not in the above two categories.
 * @param c The character to check.
 */
static inline ESCAPE escape_type(const char c)
{
    if (c == '\\' || c == '\'' || c == '\"')
        return ESCAPE_NORMAL;
    if (' ' <= c  && c <= '~')
        return ESCAPE_NONE;
    return ESCAPE_HEX;
}

/**
 * @brief Write a character to a file stream, escaping it (by writing multiple characters) if necessary.
 * @param c The character to write.
 */
static inline void fputc_escaped(char c, FILE *f)
{
    switch (escape_type(c))
    {
    case ESCAPE_NONE: fputc(c, f); break;
    case ESCAPE_NORMAL: fprintf(f, "\\%c", c); break;
    case ESCAPE_HEX: fprintf(f, "\\x%02X", (unsigned char)c); break;
    }
}

#endif // ESCAPE_H