#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**
 * Parses an email address from the beginning of a string.
 * Email conforms to the regular expression: ^[a-zA-Z._-]*@[a-zA-Z]+.[a-zA-Z]+
 * @param test_string The string to parse.
 * @return A pointer to the first character after the email address in the string. If no email address is found, the pointer will be the same as the input string.
 */
const char *parse_email_address(const char *const test_string)
{
    const char *cp = test_string;
    // Parse [a-zA-Z._-]*
    while (isalpha(*cp) || *cp == '.' || *cp == '_' || *cp == '-')
        cp++;
    // Parse @
    if (*cp == '@')
        cp++;
    else
        return test_string;
    // Parse [a-zA-Z]+
    //      Parse [a-zA-Z]
    if (isalpha(*cp))
        cp++;
    else
        return test_string;
    //      Parse [a-zA-Z]*
    while (isalpha(*cp))
        cp++;

    // Parse .
    if (*cp == '.')
        cp++;
    else
        return test_string;

    // Parse [a-zA-Z]+
    //      Parse [a-zA-Z]
    if (isalpha(*cp))
        cp++;
    else
        return test_string;
    //      Parse [a-zA-Z]*
    while (isalpha(*cp))
        cp++;
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
    int success, parsed_length;
    // Test valid email addresses
    printf("Testing valid email addresses\n");
    for (test_email = valid_emails_to_test; *test_email != NULL; test_email++)
    {
        parsed_length = parse_email_address(*test_email) - *test_email;
        success = strlen(*test_email) == parsed_length;
        printf("%s: actual vs. parsed: \"%s\", \"%.*s\"\n", success ? "success" : "failure", *test_email, parsed_length, *test_email);
    }
    // Test invalid email addresses
    printf("Testing invalid email addresses\n");
    for (test_email = invalid_emails_to_test; *test_email != NULL; test_email++)
    {
        parsed_length = parse_email_address(*test_email) - *test_email;
        success = strlen(*test_email) == parsed_length;
        printf("%s: actual vs. parsed: \"%s\", \"%.*s\"\n", success ? "success" : "failure", *test_email, parsed_length, *test_email);
    }
    return 0;
}