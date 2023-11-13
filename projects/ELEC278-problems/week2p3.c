#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct list_node
{
    // Previous node; null if this is the first node.
    struct list_node *prev;

    // Next node; null if this is the last node.
    struct list_node *next;

    int value;
};

struct list
{
    // First node; null if the list is empty.
    struct list_node *first;

    // Last node; null if the list is empty.
    struct list_node *last;
};

struct iterator
{
    // Current node; null if we are at the end of the list.
    struct list_node *current;

    // Pointer to the list data structure itself; never null.
    struct list *list;
};

// Checks whether the iterator is at the end of the list.
bool at_end(struct iterator iter)
{
    return iter.current == NULL;
}

// Gets the value at an iterator.
// Only valid if !at_end(iter).
int get(struct iterator iter)
{
    return iter.current->value;
}

// Advances an iterator to the next position.
// Returns false if the iterator is at the end of the list.
bool next(struct iterator *iter)
{
    if (iter->current == NULL)
        return false;

    iter->current = iter->current->next;
    return true;
}

// Inserts a number at a given position in a list.
void insert_at(struct iterator iter, int num)
{
    // Construct a new node to splice into the list.
    struct list_node *new_node = malloc(sizeof(struct list_node));
    new_node->value = num;

    if (iter.current == NULL)
    {
        // We are at the end of the list.
        new_node->prev = iter.list->last;
        new_node->next = NULL;
        iter.list->last = new_node;
    }
    else
    {
        new_node->prev = iter.current->prev;
        new_node->next = iter.current;
        iter.current->prev = new_node;
    }

    // Finish connecting to previous node/beginning of list
    // (same for both cases).
    if (new_node->prev == NULL)
        iter.list->first = new_node;
    else
        new_node->prev->next = new_node;
}

// Removes the element at a given position in a list.
// Returns false if the iterator is at the end of the list.
// running this function invalidates the iterator
bool remove_at(struct iterator iter)
{
    // iterator is at the end of the list
    if (iter.current == NULL)
        return false;

    // make the previous node point to the one after the current node
    if (iter.current->prev != NULL)
        iter.current->prev->next = iter.current->next;
    else
        iter.list->first = iter.current->next;
    // make the next node point to the one before the current node
    if (iter.current->next != NULL)
        iter.current->next->prev = iter.current->prev;
    else
        iter.list->last = iter.current->prev;

    // free the memory of the removed node

    free(iter.current);
    return true;
}

int main()
{
    // List is initially empty.
    struct list numbers = {.first = NULL, .last = NULL};
    struct iterator iter = {.list = &numbers, .current = numbers.first};

    // Add numbers 1 through 10 to the list, in order.
    for (int i = 1; i <= 10; i++)
    {
        insert_at(iter, i);
        next(&iter);
    }

    // Remove and print each element.
    iter.current = numbers.first;
    while (!at_end(iter))
    {
        printf("%d\n", get(iter));
        remove_at(iter);
        iter.current = numbers.first;
    }

    return 0;
}