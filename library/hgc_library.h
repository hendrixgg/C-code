#ifndef HGC_LIBRARY
#define HGC_LIBRARY

#include <inttypes.h>
#include <string.h>

// Copy entries from the source matrix into the destination matrix.
void mat_copy_2x2(uint64_t destination[2][2], uint64_t source[2][2]);

/*
Computes the n-th fibonnaci number using binary matrix exponentiation. fib(0) = 0, fib(1) = 1.

time complexity: O(log_2(n))
space complexity: O(1)
*/
uint64_t fib_mat_exp(uint64_t n);

#endif