#include <stdio.h>

// actually, this is a max-heap, not a min-heap
#include "heap.h"

// Idea, put first k elements into a heap, then for each remaining element, if it is larger than the smallest element in the min-heap, remove the smallest element and insert the new element.
// Time Complexity: O(n * log(k)), space complexity: O(k)
int kth_largest(const int *data, size_t count, size_t k)
{
    // find the kth largest element of the array at 'data' with length 'count'.
    // You may assume that k is less than or equal to count.
    // You may assume that count is greater than zero.

    // Create a max-heap with the first k elements
    struct heap h = {
        h.capacity = k,
        h.length = 0,
        h.data = malloc(sizeof(struct heap_element) * k),
    };
    for (size_t i = 0; i < k; i++)
    {
        heap_insert(&h, -data[i], NULL);
    }

    // For each remaining element, if it is larger than the smallest element in the min-heap, remove the smallest element and insert the new element.
    for (size_t i = k; i < count; i++)
    {
        if (data[i] > -h.data[0].priority)
        {
            heap_remove(&h, NULL, NULL);
            heap_insert(&h, -data[i], NULL);
        }
    }

    // Return the smallest element in the min-heap
    int result = -h.data[0].priority;
    free(h.data);
    return result;
}

int main()
{
    int example_data[] = {15, 3, -7, 9, 22, 1, 7};
    printf("3rd largest value: %d", kth_largest(example_data, 7, 3));
    return 0;
}
