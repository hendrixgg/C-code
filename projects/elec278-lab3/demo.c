#include <string.h>
#include "demo.h"
#include "stack.h"

bool check_brackets(const char *str)
{
    stack_ptr s = stack_new();

    // TODO (task 3): using the stack 's', check the brackets in the strings.
    static const char brackets[7] = "([{)]}";
    while (*str != '\0')
    {
        // check for the next bracket.
        const char *bracket = strpbrk(str, brackets);
        // did not find a bracket.
        if (bracket == NULL)
            break;
        // did find a bracket.
        // check bracket type.
        int bracket_type = strchr(brackets, *bracket) - brackets;
        // if open bracket, add to the stack.
        if (bracket_type < 3)
        {
            stack_push(s, *bracket);
        }
        // if close bracket, make sure that on the top of the stack is an open bracket of the same type.
        else
        {
            // char to store the last bracket.
            static char last_bracket;
            // if there were no other brackets in the stack, exit the loop with an unempty stack.
            if (!stack_pop(s, &last_bracket))
            {
                stack_push(s, 'a');
                break;
            }

            // if brackets don't match, exit the loop with an unempty stack.
            if (bracket_type != strchr(brackets, last_bracket) - brackets + 3)
            {
                stack_push(s, 'a');
                break;
            }
        }
        // advance to the next character
        str = bracket + 1;
    }
    char c;
    bool stack_empty = !stack_pop(s, &c);
    stack_free(s);
    return stack_empty;
}
