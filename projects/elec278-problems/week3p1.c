#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Null for an empty list.
typedef struct list_node *list;

struct list_node
{
    char *value;
    list tail;
};

typedef list *iterator;

// Inserts a string at a given position in a list.
void insert_at(iterator iter, const char *str)
{
    // Construct a new node to splice into the list.
    struct list_node *new_node = malloc(sizeof(struct list_node));
    new_node->value = strdup(str);

    // 'iter' points to the 'tail' slot where we need to insert the node.
    new_node->tail = *iter;
    *iter = new_node;
}

// Inserts a string at a given position in a list.
// Returns false if the iterator is at the end of the list.
bool remove_at(iterator iter)
{
    // Make sure we are not at the end.
    if (*iter == NULL)
        return false;

    // 'iter' points to the 'tail' slot where we need to remove the node.
    struct list_node *old_node = *iter;
    *iter = old_node->tail;
    free(old_node->value);
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
char *get(iterator iter)
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
bool find(iterator *iter, const char *target)
{
    // Loop which exists when it tries to advance past the end.
    for (; !at_end(*iter); next(iter))
    {
        // If the current node has the target value, exit successfully.
        if (strcmp(get(*iter), target) == 0)
            return true;
    }

    // We've reached the end of the list without finding the target value.
    return false;
}

list duplicate_list(list values)
{
    list new_list = NULL;
    iterator old_iter = &values;
    iterator new_iter = &new_list;
    while (!at_end(old_iter))
    {
        insert_at(new_iter, get(old_iter));
        next(&old_iter), next(&new_iter);
    }
    return new_list;
}

int main()
{
    // List is initially empty.
    list strings = NULL;
    iterator iter = &strings;

    // Insert some example strings.
    insert_at(iter, "hello");
    next(&iter);
    insert_at(iter, "test");
    next(&iter);
    insert_at(iter, "1234");

    // Duplicate the list.
    list second_copy = duplicate_list(strings);

    // Remove an element from the original.
    iter = &strings;
    next(&iter);
    remove_at(iter);

    // Remove and print each element of the original list.
    printf("original:\n");
    iter = &strings;
    while (!at_end(iter))
    {
        printf("%s\n", get(iter));
        remove_at(iter);
    }

    // Remove and print each element of the second list.
    printf("second:\n");
    iter = &second_copy;
    while (!at_end(iter))
    {
        printf("%s\n", get(iter));
        remove_at(iter);
    }

    return 0;
}
