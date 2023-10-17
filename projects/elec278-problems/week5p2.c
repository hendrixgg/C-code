#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// The size of the arrays at each node.
#define ARRAY_SIZE 4

// A linked list, where each node contains an array; null for an empty list.
typedef struct list_node *hybrid_list;

struct list_node
{
    int values[ARRAY_SIZE];
    hybrid_list tail;
};

// To transform this into a queue, if there are 4 elements or less in the queue (there is only one list_node), you would need to consider the one node as a circular array queue. You only need to maintain the offset at the front of the queue.
struct queue
{
    // Both are NULL if the stack is empty.
    hybrid_list first, last;
    // Offset of elements used in the first node (> 0). Ignore if list == NULL.
    int first_offset;
    // Number of elements used in the first node (> 0). Ignore if list == NULL.
    int first_length;
    // Number of elements used in the last node (> 0). Ignore if list == NULL.
    int last_length;
};

void enqueue(struct queue *q, int value)
{
    // If a new node is required.
    if (q->first == NULL || q->last_length == ARRAY_SIZE)
    {
        struct list_node *new_node = malloc(sizeof(struct list_node));
        new_node->tail = NULL;

        // Queue was empty.
        if (q->first == NULL)
        {
            q->first = new_node;
            q->first_length = 0;
            q->first_offset = 0;
        }
        // Queue was not empty.
        else
        {
            q->last->tail = new_node;
        }
        q->last = new_node;
        q->last_length = 0;
    }

    // Only one node.
    if (q->first == q->last)
    {
        q->first->values[(q->first_offset + q->first_length++) % ARRAY_SIZE] = value;
        q->last_length = q->first_length;
    }
    // More than one node, insert at the last node
    else
    {
        q->last->values[q->last_length++];
    }
}

bool dequeue(struct queue *q, int *out)
{
    if (q->first == NULL)
        return false;
    *out = q->first->values[q->first_offset];
    q->first_offset = (q->first_offset + 1) % ARRAY_SIZE;

    // Need to remove the first node.
    if (--q->first_length == 0)
    {
        struct list_node *old_node = q->first;
        q->first = old_node->tail;
        free(old_node);

        // The queue is now empty.
        if (q->first == NULL)
        {
            q->last == NULL;
        }
        // There is another node that becomes the first node.
        else
        {
            // Reset the offest to 0.
            q->first_offset = 0;
            // If there is only one node remaining, first_length = last_length, otherwise >=2 nodes are remaining, so the last_length = ARRAY_SIZE.
            q->first_length = q->first == q->last ? q->last_length : ARRAY_SIZE;
        }
    }
    // There is only one node.
    else if (q->first == q->last)
    {
        q->last_length = q->first_length;
    }
    return true;
}

int main()
{
    struct queue q = {
        .first = NULL,
        .last = NULL,
    };

    for (int i = 1; i <= 10; i++)
    {
        printf("push: %d\n", i);
        enqueue(&q, i);
    }

    int value;
    while (dequeue(&q, &value))
        printf("pop: %d\n", value);

    return 0;
}
