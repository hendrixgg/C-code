#include <stdio.h>

#include "hgc_library_tests.h"
#include "hgc_library.h"
int main()
{
    unit_test_results *main_results = test_fib_mat_exp(NULL);
    (void)unit_test_results_print(main_results);

    (void)printf("\n\n (2^64 - 1)-th fibonnaci number modulo 2^64 - 1: %llu\n", fib_mat_exp(-1));
    (void)unit_test_results_free(main_results);
}