#include "stack.h" // Header file this implements.

#include <stdlib.h> // for malloc, free, and realloc.
#include <assert.h> // for assert.
// #include <string.h> // for memcpy.

struct stack
{
    // Points to dynamically allocated array of size 'capacity', or is NULL if empty.
    cell_t **data;
    // The capacity of the array.
    size_t capacity;
    // The number of entries currently in the stack.
    size_t length;
};

stack_t *stack_new()
{
    stack_t *s = malloc(sizeof(struct stack));
    s->data = NULL;
    s->capacity = 0;
    s->length = 0;
    return s;
}

void stack_free(stack_t *s)
{
    if (s->data != NULL)
        free(s->data);
    free(s);
}

void stack_push(stack_t *s, cell_t *cell_ptr)
{
    assert(s != NULL);
    // Check if already at capacity, allocate more memory if needed.
    if (s->length == s->capacity)
    {
        // double the capacity
        s->capacity = s->capacity == 0 ? 1 : s->capacity << 1;
        // call realloc to store the larger amount of memory.
        s->data = realloc(s->data, s->capacity * sizeof(*s->data));
    }
    // Put the new data in the stack. Will cause undefined behaviour if the data is not the correct size.
    s->data[s->length++] = cell_ptr;
}

bool stack_pop(stack_t *s, cell_t **out)
{
    assert(s != NULL);
    // return false if there is no element to remove.
    if (s->length == 0)
        return false;

    // Copy the last element to the out variable. and decrement the length of the stack.
    if (out != NULL)
        *out = s->data[--s->length];

    // realloc to half the memory if the length has become a quarter of the capacity.
    if (s->length < (s->capacity >> 2))
    {
        // half the capacity
        s->capacity >>= 1;
        s->data = realloc(s->data, s->capacity * sizeof(*s->data));
    }
    return true;
}
