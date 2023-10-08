#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include "hgc_library_tests.h"
#include "hgc_library.h"

unit_test_results *unit_test_results_init(const char *name, uint64_t num_tests)
{
    unit_test_results *results = malloc(sizeof(unit_test_results));
    assert(results != NULL);
    (void)strncpy(results->name, name, UNIT_TEST_NAME_BUFFER_SIZE - 1);
    // ensure that the string can be printed properly incase of an overflow
    results->name[UNIT_TEST_NAME_BUFFER_SIZE - 1] = '\0';
    results->num_tests = results->test_capacity = num_tests;

    results->pass_fail = malloc(num_tests * sizeof(TEST_CASE_RESULT));
    assert(results->pass_fail != NULL);

    results->runtime = malloc(num_tests * sizeof(uint64_t));
    assert(results->runtime != NULL);

    results->memory_usage = malloc(num_tests * sizeof(uint64_t));
    assert(results->memory_usage != NULL);

    return results;
}

unit_test_results *unit_test_results_reinit(unit_test_results *results, const char *name, uint64_t num_tests)
{
    assert(results != NULL);
    (void)strncpy(results->name, name, UNIT_TEST_NAME_BUFFER_SIZE - 1);
    // ensure that the string can be printed properly incase of an overflow
    results->name[UNIT_TEST_NAME_BUFFER_SIZE - 1] = '\0';
    // Do not decrease memory allocated. This is done since it is assumed that you will use one unit_test_results struct and reuse it for multiple unit tests. If you want to deallocate the memory of unit_test_results call unit_test_results_free.
    if (results->test_capacity >= num_tests)
    {
        results->num_tests = num_tests;
        return results;
    }
    results->num_tests = results->test_capacity = num_tests;

    results->pass_fail = realloc(results->pass_fail, num_tests * sizeof(TEST_CASE_RESULT));
    assert(results->pass_fail != NULL);

    results->runtime = realloc(results->runtime, num_tests * sizeof(uint64_t));
    assert(results->runtime != NULL);

    results->memory_usage = realloc(results->memory_usage, num_tests * sizeof(uint64_t));
    assert(results->memory_usage != NULL);

    return results;
}

void unit_test_results_free(unit_test_results *results)
{
    assert(results != NULL);
    (void)free(results->pass_fail);
    (void)free(results->runtime);
    (void)free(results->memory_usage);
    (void)free(results);
}

void unit_test_results_print(unit_test_results *results)
{
    assert(results != NULL);
    uint64_t total_tests_passed = 0, total_runtime_ms = 0, max_runtime_test = 0, max_memory_usage_test = 0;
    (void)printf("Testing Function: %s\n", results->name);
    // (void)printf("\tResults ('.' = pass, '!' = fail): ");
    (void)printf("\tResults: ");
    for (uint64_t i = 0; i < results->num_tests; i++)
    {
        // Print result for test case.
        (void)putchar(results->pass_fail[i]);
        // Sum up pass/fail
        total_tests_passed += results->pass_fail[i] == TEST_CASE_PASS;
        // Sum up total runtime.
        total_runtime_ms += results->runtime[i];
        // Find max runtime test.
        max_runtime_test = results->runtime[i] > results->runtime[max_runtime_test] ? i : max_runtime_test;
        // Find max memory usage
        max_memory_usage_test = results->memory_usage[i] > results->memory_usage[max_memory_usage_test] ? i : max_memory_usage_test;
    }
    (void)printf("\n");
    (void)printf("\t%llu / %llu Tests Passed.\n", total_tests_passed, results->num_tests);
    (void)printf("\tRuntime:\n");
    (void)printf("\t\tTotal Time Taken: %llu ms\n", total_runtime_ms);
    (void)printf("\t\tMaximum Time Taken: %llu ms on test case %llu\n", results->runtime[max_runtime_test], max_runtime_test);
    // still need to calculate memory usage.
    // (void)printf("\tMemory:\n");
    // (void)printf("\t\tMaximum Memory Usage: %llu bytes on test case %llu\n", results->memory_usage[max_memory_usage_test], max_memory_usage_test);
}

#define FIB_MAT_EXP_NUM_TESTS 7
#define FIB_MAT_EXP_UNIT_TEST_NAME "uint64_t fib_mat_exp(uint64_t n);"
unit_test_results *test_fib_mat_exp(unit_test_results *results)
{
    const uint64_t num_tests = FIB_MAT_EXP_NUM_TESTS;
    if (results == NULL)
        results = unit_test_results_init(FIB_MAT_EXP_UNIT_TEST_NAME, num_tests);
    else
        results = unit_test_results_reinit(results, FIB_MAT_EXP_UNIT_TEST_NAME, num_tests);

    // test cases
    uint64_t fib_input_output[FIB_MAT_EXP_NUM_TESTS][2] = {
        {0, 0},
        {1, 1},
        {2, 1},
        {10, 55},
        {44, 701408733ULL},
        {93, 12200160415121876738ULL},
        {(uint64_t)-1, 800812746651928290ULL},
    };
    clock_t timer;
    for (uint64_t i = 0, test_case; i < num_tests; i++)
    {
        // memory usage ????

        // timing start
        timer = clock();
        test_case = fib_mat_exp(fib_input_output[i][0]);
        timer = clock() - timer;
        // timing end

        // pass/fail
        results->pass_fail[i] = (test_case == fib_input_output[i][1]) * TEST_CASE_PASS + (test_case != fib_input_output[i][1]) * TEST_CASE_FAIL;
        // runtime
        results->runtime[i] = (double)timer / (CLOCKS_PER_SEC);
    }
    return results;
}