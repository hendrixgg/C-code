#include <stdio.h>
#include <string.h>

const char *parse_input(const char *const input)
{
    const char *p = input;
    // 0: start
    // >0: number of 'a's
    // <0: number of 'b's
    char state = 0;
    while (*p == 'a' || *p == 'b')
    {
        state += (*p == 'a') - (*p == 'b');
        p++;
    }
    if (*p == '\0' && (-3 <= state && state <= 3))
        return p;

    return input;
}

int main()
{
    const char *const input_strings[] = {
        "aaa",
        "abab",
        "aaaabbb",
        "aaaaaaa",
        "bbbbbbb",
        "aaaaaaabbbbbbbb",
        "bbbb",
        "aaaabbbbaaaa",
        "bbbaaaabbbb",
        "aaaaaaaabbbbbbbb",
        "abababab",
        "ababababab",
        "aaabbbbaa",
        "abbbaaaa",
        "bbbaaabbb",
        "",
        "a",
        "b",
        "aaabbb",
        "abababababab",
        NULL,
    };
    const char *const *test_string;
    printf("|Input String|Accept/Reject|\n"
           "|------------|-------------|\n");
    int success;
    for (test_string = input_strings; *test_string != NULL; test_string++)
    {
        success = strlen(*test_string) == (parse_input(*test_string) - *test_string);
        printf("|%s|%s|\n", *test_string, success ? "Accept" : "Reject");
    }
    return 0;
}