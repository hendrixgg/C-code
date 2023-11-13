#ifndef HGC_LIBRARY_TESTS
#define HGC_LIBRARY_TESTS

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

/* need to test for the following metrics of a function:
- runtime
- memory usage (don't know how to do this)
- correctness

what should be returned for each unit test?
- uint64_t for number of unit tests
- char array of pass/fail for each test case
- uint64_t array of runtime taken for each test case
- uint64_t array containing total number of bytes
- float for pass %
- uint64_t for max runtime on a test case
- uint64_t for max memory usage on a test case
*/

#define UNIT_TEST_NAME_BUFFER_SIZE 64
typedef enum _TEST_CASE_RESULT
{
    TEST_CASE_PASS = '.',
    TEST_CASE_FAIL = '!',
} TEST_CASE_RESULT;
// currently not storing runtime or memory usage, only pass/fail.
typedef struct _unit_test_results
{
    // Null terminating string which holds the name of the unit test.
    char name[UNIT_TEST_NAME_BUFFER_SIZE];
    // Number of test cases for which memory has been allocated.
    uint64_t test_capacity;
    // Number of test cases in this unit test.
    uint64_t num_tests;
    // Dynamically allocated array for the result each test case.
    TEST_CASE_RESULT *pass_fail;
    // Dynamically allocated array for the runtime (in ns) for each test case
    uint64_t *runtime;
    // Dynamically allocated array for the memory usage (in bytes) for each test case
    uint64_t *memory_usage;
} unit_test_results;

unit_test_results *unit_test_results_init(const char *name, uint64_t num_tests);
unit_test_results *unit_test_results_reinit(unit_test_results *results, const char *name, uint64_t num_tests);
void unit_test_results_free(unit_test_results *results);
void unit_test_results_print(unit_test_results *results);

// if you want to use an existing struct to store the test results, pass in a pointer to that struct. A pointer to the final test results will always be returned.
unit_test_results *test_fib_mat_exp(unit_test_results *results);

#endif