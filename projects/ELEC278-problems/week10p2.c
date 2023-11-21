#include <stdio.h>
#include <string.h>

// This is actually a max-heap, not a min-heap
#include "heap.h"

// Idea, put first m elements into a heap (first from each array), then for each remaining element, if it is larger than the smallest element in the min-heap, put the smallest element in the output array, remove it from the heap and insert the new element.
// Time complexity: O(m * n * log(m)), space complexity: O(m)
void merge_sorted_lists(int *out, const int **in, size_t m, size_t n)
{
    // merge the 'm' sorted arrays of length 'n', given by 'in', into the single array given by 'out'.
    // You may assume that 'out' has enough space to store all elements.
    // You may assume that 'm' is greater than zero.
    // You may assume that 'n' is greater than zero.
    // You may assume that 'in' is an array of 'm' pointers to arrays of length 'n'.

    // Create a min-heap with the first element of each array
    struct heap h = {
        .capacity = m,
        .length = 0,
        .data = malloc(sizeof(struct heap_element) * m),
    };
    // Insert first element from each array.
    for (size_t i = 0; i < m; i++)
        heap_insert(&h, -in[i][0], (const char *)i);

    int value;
    size_t out_index = 0, in_index[m], curr_array;
    memset(in_index, 0, sizeof(in_index));
    // While the heap is not empty, remove the smallest element (along with the array that it came from) and insert the next element from the array it came from
    while (heap_remove(&h, &value, (const char **)&curr_array))
    {
        // Put the value into the output array.
        out[out_index++] = -value;
        // Insert the next element from the array it came from. If we are at the end of the array, then don't insert anything.
        if (in_index[curr_array] < n - 1)
            heap_insert(&h, -in[curr_array][++in_index[curr_array]], (const char *)curr_array);
    }
}

int main()
{
    int first_array[] = {3, 9, 10, 13, 27};
    int second_array[] = {0, 19, 20, 23, 29};
    int third_array[] = {2, 6, 8, 15, 19};

    const int *input[] = {first_array, second_array, third_array};
    int output[15];
    merge_sorted_lists(output, input, 3, 5);

    for (size_t i = 0; i < 15; i++)
        printf("%d, ", output[i]);

    return 0;
}
