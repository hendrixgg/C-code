/**
 * An interface for stacks containing characters.
 */

#ifndef LAB3_STACK_H
#define LAB3_STACK_H

#include "model.h" // for cell_t.

#include <stdbool.h> // for bool.
#include <stddef.h>  // for size_t.

typedef struct stack stack_t;

/**
 * Creates an empty stack. The stack pointer should be passed to stack_free when it is no longer needed, to clean up its allocations.
 * @return A pointer to a new stack.
 */
stack_t *stack_new();
//  * @param element_size The number of bytes to store each element in the stack.

/**
 * @param s The stack to query.
 * @return The number of bytes used to store each element in the stack.
 */
// size_t stack_element_size(const stack_t *s);

/**
 * Destroys a stack, freeing all allocated memory.
 * @param s The stack to destroy.
 */
void stack_free(stack_t *s);

/**
 * Adds an entry to a stack.
 * @param s The stack to add to.
 * @param c The cell pointer to be stored on thr stack.
 */
void stack_push(stack_t *s, cell_t *c);

/**
 * Removes an entry from a stack, optionally storing the removed value in '*out'.
 *
 * @param s The stack to remove from.
 * @param out A pointer to an address to store the removed data. If out is `NULL`, then nothing is stored. Otherwise, *out must be a valid pointer to a `cell_t *`.
 * @return True if an entry was removed, false if the stack was empty.
 */
bool stack_pop(stack_t *s, cell_t **out);

#endif // LAB3_STACK_H