#include <stdio.h>
#include <assert.h>

#define SIZE_(arr) (sizeof(arr) / sizeof(*arr))

int array_max(const int *array, size_t count)
{
    assert(array != NULL && count >= 1);
    // return the maximum of the 'count' elements of 'array'.
    // will cause an error if the array is empty
    int max = array[0];
    for (int i = 1; i < count; i++)
    {
        if (array[i] > max)
        {
            max = array[i];
        }
    }
    return max;
}

int array_average(const int *array, size_t count)
{
    // return the average of the 'count' elements of 'array'.
    int sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += array[i];
    }
    return sum / count;
}

int main()
{
    // construct an array, call 'array_average' on it, and print the result.
    // 32 total sum, 5 numbers in the sum, 32/5 = 6.4
    const int array[] = {1, 2, 20, 4, 5};
    printf("Average: %d\n", array_average(array, SIZE_(array)));
    printf("Max: %d\n", array_max(array, SIZE_(array)));
    printf("Hello, World!\n");
    return 0;
}
