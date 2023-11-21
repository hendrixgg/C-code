#ifndef TUTORIAL10_HEAP_H
#define TUTORIAL10_HEAP_H

#include <stdbool.h>
#include <stdlib.h>

struct heap_element
{
    int priority;
    const char *data;
};

// stores a max-heap of heap_elements
struct heap
{
    struct heap_element *data;
    size_t capacity, length;
};

void heap_insert(struct heap *h, int priority, const char *data);
bool heap_remove(struct heap *h, int *priority_out, const char **data_out);

#endif // TUTORIAL10_HEAP_H
