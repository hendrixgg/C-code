// Written by Hendrix Gryspeerdt for CMPE223 Assignment 2

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define DA_DEFINE(name, type)   \
typedef struct _##name {                \
    type *items;                \
    size_t capacity;            \
    size_t count;               \
} name

// 
/**
 * 
 * @param da The pointer to dynamic array struct to initialize. (must have `items`, `count`, and `capacity` fields).
 */
#define da_init(da) do { (da)->items = NULL; (da)->capacity = 0; (da)->count = 0; } while (0)


//
/**
 * Ensure enough memory is allocated before pushing an item to the dynamic array. Useful when you want to assign to the last element of the array directly.
 * @param da The pointer to dynamic array struct to pre-push to. (must have `items`, `count`, and `capacity` fields).
 */
#define da_pre_push(da)                                                         \
    do {                                                                        \
        if ((da)->count >= (da)->capacity) {                                    \
            if ((da)->capacity == 0) (da)->capacity = 1;                        \
            else (da)->capacity *= 2;                                           \
            void *ptr = realloc((da)->items, (da)->capacity * sizeof(*(da)->items));\
            if (ptr == NULL) {                                                  \
                perror("realloc");                                              \
                free((da)->items);                                              \
                exit(EXIT_FAILURE);                                             \
            }                                                                   \
            (da)->items = ptr;                                                  \
        }                                                                       \
    } while (0)

// 
/**
 * Push (append) an item to the end of the dynamic array.
 * 
 * @param da The pointer to dynamic array struct to push to. (must have `items`, `count`, and `capacity` fields).
 * @param item The item to push.
 */
#define da_push(da, item)                       \
    do                                          \
    {                                           \
        da_pre_push(da);                        \
        (da)->items[(da)->count++] = (item);    \
    } while (0)

//
/**
 * Pop an item from the end of the dynamic array.
 * 
 * @param da The pointer to dynamic array struct to pop from.
 */
#define da_pop(da) ((da)->count > 0) ? (da)->items[--(da)->count] : 0

// 
/**
 * Clear the dynamic array, freeing the items and setting the count and capacity to 0.
 * 
 * @param da The pointer to dynamic array struct to clear. (must have `items`, `count`, and `capacity` fields).
 */
#define da_clear(da)                            \
    do {                                        \
        free((da)->items);                      \
        (da)->items = NULL;                     \
        (da)->count = 0;                        \
        (da)->capacity = 0;                     \
    } while (0)



/*
Assumptions: 
- input is a null-terminated string.
- if string contains a character other than '0', '1', or 'c', then the string is rejected.
*/
bool pda_accept(const char *s) {
    DA_DEFINE(da_char, char);
    da_char stack;
    da_init(&stack);
    enum Q {q1, q2, q3, q4} state = q1;
    bool done = false;
    do {
        switch (state) {
            case q1:
                da_push(&stack, '$');
                state = q2;
                break;
            case q2:
                switch (*s) {
                    case '0':
                        da_push(&stack, '0');
                        state = q2;
                        ++s;
                        break;
                    case '1':
                        da_push(&stack, '1');
                        state = q2;
                        ++s;
                        break;
                    case 'c':
                        state = q3;
                        ++s;
                        break;
                    default:
                        done = true;
                        break;
                }
                break;
            case q3:
                switch (da_pop(&stack)) {
                    case '0':
                        switch(*s) {
                            case '0':
                                state = q3;
                                ++s;
                                break;
                            default:
                                done = true;
                                break;
                        }
                        break;
                    case '1':
                        switch(*s) {
                            case '1':
                                state = q3;
                                ++s;
                                break;
                            default:
                                done = true;
                                break;
                        }
                        break;
                    case '$':
                        state = q4;
                        break;
                    default:
                        done = true;
                        break;
                }
                break;
            case q4:
                done = true;
                break;
        }
    } while (!done);
    da_clear(&stack);
    return state == q4 && *s == '\0';
}

#define accept_reject(x) (x) ? "accept" : "reject"

struct test_case {
    const char *const s;
    const bool accepted;
};

void test_pda_accept(FILE *stream, const struct test_case *const test_cases, const size_t num_test_cases) {
    size_t test_cases_passed = 0;
    for (size_t i = 0; i < num_test_cases; ++i) {
        const struct test_case *tc = &test_cases[i];
        const bool result = pda_accept(tc->s);
        const bool correct = (result == tc->accepted);
        test_cases_passed += correct;
        fprintf(stream, "Test case %llu/%llu %s: expected %s, got %s, on input \"%s\"\n", 
            i+1, num_test_cases, correct ? "SUCCESS" : "FAILURE", accept_reject(tc->accepted), accept_reject(result), tc->s);
    }
    fprintf(stream, "%llu/%llu test cases passed.\n", test_cases_passed, num_test_cases);
}

int main() {
    const struct test_case test_cases[] = {
        {"10110c01101", true},
        {"10010c01101", false},
        {"23498f3", false},
        {"000cc000", false},
        {"01010101c01010101", false},
        {"01010101c10101010", true},
        {"0110001c1000110", true},
        // you can add more test cases here
    };
    test_pda_accept(stdout, test_cases, sizeof(test_cases) / sizeof(test_cases[0]));
    return 0;
}