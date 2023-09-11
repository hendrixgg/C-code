#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Null for an empty list.
typedef struct list_node *list;

struct list_node
{
    int value;
    list tail;
};

typedef list *iterator;

// Inserts a number at a given position in a list.
void insert_at(iterator iter, int num)
{
    // Construct a new node to splice into the list.
    struct list_node *new_node = malloc(sizeof(struct list_node));
    new_node->value = num;

    // 'iter' points to the 'tail' slot where we need to insert the node.
    new_node->tail = *iter;
    *iter = new_node;
}

// Removes a number at a given position in a list.
// Returns false if the iterator is at the end of the list.
bool remove_at(iterator iter)
{
    // Make sure we are not at the end.
    if (*iter == NULL)
        return false;

    // 'iter' points to the 'tail' slot where we need to remove the node.
    struct list_node *old_node = *iter;
    *iter = old_node->tail;
    free(old_node);
    return true;
}

// Checks whether the iterator is at the end of the list.
bool at_end(iterator iter)
{
    return *iter == NULL;
}

// Gets the value at an iterator.
// Only valid if !at_end(iter).
int get(iterator iter)
{
    return (*iter)->value;
}

// Advances an iterator to the next position.
// Returns false if the iterator is at the end of the list.
bool next(iterator *iter)
{
    if (at_end(*iter))
        return false;

    *iter = &(**iter)->tail;
    return true;
}

// Advances an iterator until it reaches the target value.
// Returns false if the end of the list is reached.
bool find(iterator *iter, int target)
{
    // Loop which exists when it tries to advance past the end.
    for (; !at_end(*iter); next(iter))
    {
        // If the current node has the target value, exit successfully.
        if (get(*iter) == target)
            return true;
    }

    // We've reached the end of the list without finding the target value.
    return false;
}

// Reverses the order of elements in the list.
void reverse(list *nums)
{
    // return early if NULL pointer.
    if (nums == NULL)
        return;

    list previous = NULL, tail;

    while (*nums != NULL)
    {
        // Grabbing the tail of the current node.
        tail = (*nums)->tail;
        // Making the current node point to the one behind it
        (*nums)->tail = previous;
        // shift things over by one
        previous = (*nums);
        (*nums) = tail;
    }
    // Recovering the last node since the loop exits as previous points to the head of the list.
    (*nums) = previous;
}

int main()
{
    // List is initially empty.
    list numbers = NULL;
    iterator iter = &numbers;

    // Add numbers 1 through 10 to the list, in order.
    for (int i = 1; i <= 10; i++)
    {
        insert_at(iter, i);
        next(&iter);
    }

    // Print each element.
    iter = &numbers;
    while (!at_end(iter))
    {
        printf("%d\n", get(iter));
        next(&iter);
    }

    // Reverse the list.
    reverse(&numbers);
    printf("reverse\n");

    // Remove and print each element.
    iter = &numbers;
    while (!at_end(iter))
    {
        printf("%d\n", get(iter));
        remove_at(iter);
    }

    return 0;
}
