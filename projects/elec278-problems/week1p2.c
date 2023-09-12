#include <stdio.h>

#define SIZE_(array) (sizeof(array) / sizeof(*array))

// Always '0' through '9'.
typedef char digit;

// Numeric values match T1 form.
enum marital
{
    MARRIED = 1,
    COMMON_LAW = 2,
    WIDOWED = 3,
    DIVORCED = 4,
    SEPARATED = 5,
    SINGLE = 6,
};

struct numeric_date
{
    digit year[4];
    digit month[2];
    digit day[2];
};

struct tax_info
{
    // Social insurance number.
    digit sin[9];
    // Always a valid date.
    struct numeric_date date_of_birth;
    // Always a valid date, or all zeros if empty.
    struct numeric_date date_of_death;
    // Always a valid enum value.
    enum marital marital_status;
};

size_t numeric_to_int(const digit *numeric, int length)
{
    unsigned int result = 0;
    for (size_t i = 0; i < length; i++)
    {
        result = result * 10 + numeric[i];
    }
    return result;
}

const char *parse_date(const struct numeric_date *date)
{
    // check if date is blank
    if (date->day[0] == 0 && date->day[1] == 0)
    {
        return "BLANK";
    }
    static char date_string[11];
    sprintf(date_string, "%u-%u-%u",
            numeric_to_int(date->year, SIZE_(date->year)),
            numeric_to_int(date->month, SIZE_(date->month)),
            numeric_to_int(date->day, SIZE_(date->day)));
    return date_string;
}

char *marital_to_string(enum marital status)
{
    switch (status)
    {
    case MARRIED:
        return "Married";
    case COMMON_LAW:
        return "Common law";
    case WIDOWED:
        return "Widowed";
    case DIVORCED:
        return "Divorced";
    case SEPARATED:
        return "Separated";
    case SINGLE:
        return "Single";
    default:
        return "Unknown";
    }
}

int main()
{
    struct tax_info my_tax_info = {
        .sin = {1, 2, 3, 4, 5, 6, 7, 8, 9},
        .date_of_birth = {
            .year = {1, 9, 9, 7},
            .month = {0, 3},
            .day = {2, 9},
        },
        .date_of_death = {
            .year = {0, 0, 0, 0},
            .month = {0, 0},
            .day = {0, 0},
        },
        .marital_status = SINGLE,
    };

    // print out the contents of the form.
    printf("SIN: %u\n", numeric_to_int(my_tax_info.sin, SIZE_(my_tax_info.sin)));
    printf("Date of birth: %s\n", parse_date(&my_tax_info.date_of_birth));
    printf("Date of birth: %s\n", parse_date(&my_tax_info.date_of_death));
    printf("Marital status: %s\n", marital_to_string(my_tax_info.marital_status));

    return 0;
}
