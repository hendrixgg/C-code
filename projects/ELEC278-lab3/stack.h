/**
 * An interface for stacks containing characters.
 */

#ifndef LAB3_STACK_H
#define LAB3_STACK_H

#include <stdbool.h>

typedef struct stack *stack_ptr;

// Creates an empty stack.
// The stack pointer should be passed to stack_free when it is no longer needed,
// to clean up its allocations.
stack_ptr stack_new();

// Destroys a stack, freeing all allocated memory.
void stack_free(stack_ptr s);

// Adds an entry to a stack.
void stack_push(stack_ptr s, char c);

// Removes an entry from a stack, storing the removed value in '*out'.
// Returns false if the stack is empty.
bool stack_pop(stack_ptr s, char *out);

#endif //LAB3_STACK_H
