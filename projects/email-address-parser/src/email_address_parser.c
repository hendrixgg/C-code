#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define evalIf(p, e) ((p) ? e : p)
const char *parse_char(const char c, const char *cp)
{
    // return evalIf((*cp == c), cp + 1U);
    return (const char *)((*cp == c) * ((size_t)cp + 1U));
}
const char *parse_char_pred(bool (*pred)(char), const char *cp)
{
    // return evalIf(pred(*cp), cp + 1U);
    return (const char *)(pred(*cp) * ((size_t)cp + 1U));
}
const char *parse_char_star(bool (*pred)(char), const char *cp)
{
    while (pred(*cp))
        cp++;
    return cp;
}
const char *parse_char_plus(bool (*pred)(char), const char *cp)
{
    // return evalIf(pred(*cp), parse_char_star(cp + 1U, pred));
    return (const char *)(pred(*cp) * (size_t)parse_char_star(pred, cp + 1));
}
static inline bool is_local_char(char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '.' || c == '_' || c == '-';
}
static inline bool is_domain_char(char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

/**
 * Parses an email address from the beginning of a string.
 * Email address conforms to the regular expression: ^[a-zA-Z._-]*@[a-zA-Z]+\.[a-zA-Z]+$
 * @param s The string to parse.
 * @return A pointer to the first character after the email address in the string. If no email address is found, then a NULL pointer is returned.
 */
const char *parse_email_address(const char *cp)
{
    cp = evalIf(cp, parse_char_star(is_local_char, cp));
    cp = evalIf(cp, parse_char('@', cp));
    cp = evalIf(cp, parse_char_plus(is_domain_char, cp));
    cp = evalIf(cp, parse_char('.', cp));
    cp = evalIf(cp, parse_char_plus(is_domain_char, cp));
    return cp;
}

// Test the email address parser
int main()
{
    const char *const valid_emails_to_test[] = {
        "t_es__._t.-em...ail@example.com",
        "user.name._._under@domain.co",
        "fir-st.la-st@sub.domain",
        "ema--il@domain.com",
        "@emptyusername.com",
        NULL,
    };
    const char *const invalid_emails_to_test[] = {
        "*email@domain.com",
        "first.last@sub.sub.domain.com",
        "plainaddress",
        "username@.com",
        "username@domain..com",
        "username@domain,com",
        "username@domain@domain.com",
        "username@domain.com.",
        "#se&name@.com",
        NULL,
    };
    const char *const *test_email;
    const char *parse_result;
    int success, parsed_length;
    // Test valid email addresses
    printf("Testing valid email addresses\n");
    for (test_email = valid_emails_to_test; *test_email != NULL; test_email++)
    {
        parse_result = parse_email_address(*test_email);
        parsed_length = parse_result == NULL ? 0 : parse_result - *test_email;
        success = strlen(*test_email) == parsed_length;
        printf("%s: actual vs. parsed: \"%s\", \"%.*s\"\n", success ? "success" : "failure", *test_email, parsed_length, *test_email);
    }
    // Test invalid email addresses
    printf("Testing invalid email addresses\n");
    for (test_email = invalid_emails_to_test; *test_email != NULL; test_email++)
    {
        parse_result = parse_email_address(*test_email);
        parsed_length = parse_result == NULL ? 0 : parse_result - *test_email;
        success = strlen(*test_email) == parsed_length;
        printf("%s: actual vs. parsed: \"%s\", \"%.*s\"\n", success ? "success" : "failure", *test_email, parsed_length, *test_email);
    }
    return 0;
}