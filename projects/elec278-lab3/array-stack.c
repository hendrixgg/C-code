#include <malloc.h>
#include <assert.h>
#include "stack.h"

struct stack
{
    // Points to dynamically allocated array of size 'capacity', or is NULL.
    char *data;
    // The capacity of the array.
    size_t capacity;
    // The number of entries currently in the stack.
    size_t length;
};

stack_ptr stack_new()
{
    stack_ptr s = malloc(sizeof(struct stack));
    s->data = NULL;
    s->capacity = 0;
    s->length = 0;
    return s;
}

void stack_free(stack_ptr s)
{
    if (s->data != NULL)
        free(s->data);
    free(s);
}

void stack_push(stack_ptr s, char c)
{
    // TODO (task 2): how do we push an entry onto the stack?
    assert(s != NULL);
    // Check if already at capacity, allocate more memory if needed.
    if (s->capacity == 0)
    {
        s->data = malloc(sizeof(*s->data));
        s->capacity = 1;
    }
    else if (s->length == s->capacity)
    {
        // double the capacity
        s->capacity <<= 1;
        // call realloc to store the larger amount of memory.
        s->data = realloc(s->data, s->capacity * (sizeof(*s->data)));
    }
    // put the new character in the stack.
    s->data[s->length++] = c;
}

bool stack_pop(stack_ptr s, char *out)
{
    // TODO (task 2): how do we pop an entry from the stack?
    assert(s != NULL);
    // return false if there is no element to remove.
    if (s->length == 0)
        return false;

    // assign the last element to the out variable. and decrement the length of the stack.
    *out = s->data[--(s->length)];

    // realloc to less memory if the length has become a quarter of the capacity.
    if (s->length < (s->capacity >> 2))
    {
        // half the capacity
        s->capacity >>= 1;
        s->data = realloc(s->data, s->capacity * (sizeof(*s->data)));
    }
    return true;
}
