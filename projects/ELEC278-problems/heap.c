#include "heap.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Computes the index of the left child, given the parent index.
static size_t left_child_of(size_t index)
{
    return 2 * index + 1;
}

// Computes the index of the right child, given the parent index.
static size_t right_child_of(size_t index)
{
    return 2 * index + 2;
}

// Computes the index of the parent, given a child index.
static size_t parent_of(size_t index)
{
    return (index - 1) / 2;
}

// Swap two heap elements.
static void heap_swap(struct heap_element *a, struct heap_element *b)
{
    struct heap_element temp = *a;
    *a = *b;
    *b = temp;
}

// Move the element at the given index up the heap until it is in the correct position.
// All ancestors of the given element must be correctly ordered; descendants of the element are ignored.
static void heap_up(struct heap *h, size_t index)
{
    // Loop until we reach the root element.
    while (index > 0)
    {
        size_t parent = parent_of(index);

        // If the element is correctly ordered with respect to its parent, then it is in the correct position.
        if (h->data[parent].priority >= h->data[index].priority)
            break;

        // If it is incorrectly ordered, swap with its parent then continue.
        heap_swap(&h->data[index], &h->data[parent]);
        index = parent;
    }
}

// Move the element at the given index down the heap until it is in the correct position.
// All descendants of the given element must be correctly ordered; ancestors of the element are ignored.
static void heap_down(struct heap *h, size_t index)
{
    while (true)
    {
        // If the left child does not exist, then neither does the right, so exit.
        size_t left_child = left_child_of(index);
        if (left_child >= h->length)
            break;

        // If the right child doesn't exist, or if the left child is higher priority, then only look at the left child.
        size_t right_child = right_child_of(index);
        if (right_child >= h->length || h->data[left_child].priority > h->data[right_child].priority)
        {
            // If the element is incorrectly ordered with respect to the left child, then swap and continue.
            if (h->data[index].priority < h->data[left_child].priority)
            {
                heap_swap(&h->data[index], &h->data[left_child]);
                index = left_child;
            }
            else
                break;
        }
        else if (h->data[index].priority < h->data[right_child].priority)
        {
            // Here, the element is incorrectly ordered with respect to the right child, so swap and continue.
            heap_swap(&h->data[index], &h->data[right_child]);
            index = right_child;
        }
        else
            break;
    }
}

void heap_insert(struct heap *h, int priority, const char *data)
{
    // Allocate or re-allocate as needed.
    if (h->capacity == 0)
    {
        h->capacity = 1;
        h->data = malloc(sizeof(struct heap_element));
    }
    else if (h->length == h->capacity)
    {
        h->capacity *= 2;
        h->data = realloc(h->data, h->capacity * sizeof(struct heap_element));
    }

    // Set up new element.
    struct heap_element el = {
        .priority = priority,
        .data = data,
    };

    // Insert new element at bottom of heap, then heap-up to correctly place it.
    size_t index = h->length++;
    h->data[index] = el;
    heap_up(h, index);
}

bool heap_remove(struct heap *h, int *priority_out, const char **data_out)
{
    // Check if the heap is empty.
    if (h->length == 0)
        return false;

    // Extract root element.
    if (priority_out != NULL)
        *priority_out = h->data[0].priority;
    if (data_out != NULL)
        *data_out = h->data[0].data;

    // Move last element to root, then heap-down to correctly place it.
    h->data[0] = h->data[--h->length];
    heap_down(h, 0);

    // Shrink allocation if applicable.
    if (h->length == 0)
    {
        h->capacity = 0;
        free(h->data);
        h->data = NULL;
    }
    else if (h->length * 2 < h->capacity)
    {
        h->capacity /= 2;
        h->data = realloc(h->data, h->capacity * sizeof(struct heap_element));
    }

    return true;
}
