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

unsigned int numeric_to_int(digit *numeric, int length)
{
    unsigned int result = 0;
    for (size_t i = 0; i < length; i++)
    {
        result = result * 10 + numeric[i];
    }
    return result;
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
    printf("Date of birth: %d-%d-%d\n",
           numeric_to_int(my_tax_info.date_of_birth.year, SIZE_(my_tax_info.date_of_birth.year)),
           numeric_to_int(my_tax_info.date_of_birth.month, SIZE_(my_tax_info.date_of_birth.month)),
           numeric_to_int(my_tax_info.date_of_birth.day, SIZE_(my_tax_info.date_of_birth.day)));
    printf("Date of death: %d-%d-%d\n",
           numeric_to_int(my_tax_info.date_of_death.year, SIZE_(my_tax_info.date_of_death.year)),
           numeric_to_int(my_tax_info.date_of_death.month, SIZE_(my_tax_info.date_of_death.month)),
           numeric_to_int(my_tax_info.date_of_death.day, SIZE_(my_tax_info.date_of_death.day)));
    printf("Marital status: %s\n", marital_to_string(my_tax_info.marital_status));

    return 0;
}
