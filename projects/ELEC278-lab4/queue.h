#ifndef LAB4_QUEUE_H
#define LAB4_QUEUE_H

#include <stdbool.h>
#include <stdlib.h>

// Dynamically-sized circular queue of integers.
struct queue {
    // Pointer to array of elements. May be null if capacity = 0.
    int *data;
    // Data starts at 'offset', consists of 'length' integers, and wraps around
    // at the end of the array, given by 'capacity'.
    size_t capacity, offset, length;
};

// Initializes the queue to be empty.
void init_queue(struct queue *q);

// Clears the queue contents, deallocating all memory.
void clear_queue(struct queue *q);

// Adds an element to the queue.
void enqueue(struct queue *q, int value);

// Removes an element from the queue, storing it in '*out'.
// Returns false if the queue was empty.
bool dequeue(struct queue *q, int *out);

// Checks whether the queue is empty.
bool queue_empty(struct queue *q);

#endif //LAB4_QUEUE_H
