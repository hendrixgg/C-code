#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "hgc_library.h"

void mat_copy_2x2(uint64_t destination[2][2], uint64_t source[2][2])
{
    const uint8_t rows = 2, cols = 2;
    assert(destination != NULL && source != NULL);
    for (uint64_t i = 0; i < rows; i++)
    {
        assert(destination[i] != NULL && source[i] != NULL);
        for (uint64_t j = 0; j < cols; j++)
        {
            destination[i][j] = source[i][j];
        }
    }
}

void mat_mult_2x2(uint64_t destination[2][2], uint64_t source[2][2])
{
    uint64_t temp[2][2];
    temp[0][0] = destination[0][0] * source[0][0] + destination[0][1] * source[1][0];
    temp[0][1] = destination[0][0] * source[0][1] + destination[0][1] * source[1][1];
    temp[1][0] = destination[1][0] * source[0][0] + destination[1][1] * source[1][0];
    temp[1][1] = destination[1][0] * source[0][1] + destination[1][1] * source[1][1];
    (void)mat_copy_2x2(destination, temp);
}

uint64_t fib_mat_exp(uint64_t n)
{
    /*
    representing:
    f_n     = 0 * f_(n-1) + 1 * f_n
    f_(n+1) = 1 * f_(n-1) + 1 * f_n

    f_n = result[0][1]
    */
    uint64_t fib_mat[2][2] = {
        {0, 1},
        {1, 1}};
    // Identity matrix.
    uint64_t result[2][2] = {
        {1, 0},
        {0, 1}};

    for (; n > 0; n >>= 1)
    {
        // result *= fib_mat;
        if (n & 1)
            (void)mat_mult_2x2(result, fib_mat);

        // fib_mat *= fib_mat;
        (void)mat_mult_2x2(fib_mat, fib_mat);
    }
    return result[0][1];
}
