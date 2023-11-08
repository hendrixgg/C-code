#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "queue.h"

void init_queue(struct queue *q)
{
    assert(q != NULL);
    q->data = NULL;
    q->capacity = 0;
    q->offset = 0;
    q->length = 0;
}

void clear_queue(struct queue *q)
{
    assert(q != NULL);
    if (q->data != NULL)
    {
        free(q->data);
        init_queue(q);
    }
}

void enqueue(struct queue *q, int value)
{
    assert(q != NULL);
    // This subtraction is fine since q->capacity >= q->length is always true.
    switch (q->capacity - q->length)
    {
    // Need to increase size of queue. q->capacity - q->length == 0 ==> q->capacity == q->length.
    case 0:
        // Allocate double the existing capacity, +1 if the capacity was zero.
        q->data = realloc(q->data, sizeof(*q->data) * ((q->capacity <<= 1) + (q->capacity == 0)));
        q->capacity += (q->capacity == 0);
        // Move wraparound data to the correct position.
        // Move the first (offset + length - capacity/2) elements to the end of the list
        memcpy(q->data + (q->capacity >> 1), q->data, sizeof(*q->data) * (q->offset + q->length - (q->capacity >> 1)));
    // There is space to enqueue this element.
    default:
        // Put the new element where it's supposed to go. After, increment the length.
        q->data[(q->offset + q->length++) % q->capacity] = value;
        break;
    }
}

bool dequeue(struct queue *q, int *out)
{
    assert(q != NULL);
    if (q->length == 0)
        return false;

    // Extract the element to be removed
    *out = q->data[q->offset++];
    q->offset -= (q->offset == q->capacity) * q->capacity;
    // normally: q->capacity / 4 < q->length <= q->capacity / 2
    // Decrease the capacity if q->length == q->capacity / 4.
    // Subtract one from the length to account for the element that was just removed.
    if (--q->length == (q->capacity >> 2))
    {
        // Deal with wraparound elements.
        if (q->offset + q->length > q->capacity)
        {
            // Put elements that are at the end of array, just before the middle.
            memcpy(q->data + q->offset - (q->capacity >> 1), q->data + q->offset, sizeof(*q->data) * (q->capacity - q->offset));
            q->offset -= (q->capacity >> 1);
        }
        // Deal with elements that are past the middle.
        else if (q->offset + q->length > (q->capacity >> 1))
        {
            // Put elements that are just past the middle of the array, at the beginning.
            memcpy(q->data, q->data + (q->capacity >> 1), sizeof(*q->data) * (q->offset + q->length - (q->capacity >> 1)));
            // Shift the offset if it was past the middle of the array.
            q->offset -= (q->offset >= (q->capacity >> 1)) * (q->capacity >> 1);
        }
        // Shrink the capacity.
        q->data = realloc(q->data, sizeof(*q->data) * (q->capacity >>= 1));
    }

    return true;
}

bool queue_empty(struct queue *q)
{
    assert(q != NULL);
    return q->length == 0;
}
