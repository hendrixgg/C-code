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

// Inserts a number at a given position in a list.
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

list parse_list(const char *str)
{
    list values = NULL;
    iterator iter = &values;

    while (*str != '\0')
    {
        // Read the next number.
        char *end;
        int value = (int)strtol(str, &end, 10);
        if (end == str)
        {
            printf("invalid character: %c\n", *str);
            exit(1);
        }

        // Add the number to the list.
        insert_at(iter, value);
        next(&iter);
        str = end;

        // Skip past any amount of whitespace followed by a comma.
        while (*str != '\0' && *str != ',')
        {
            if (*str != ' ' && *str != '\n')
            {
                printf("invalid character: %c\n", *str);
                exit(1);
            }
            str++;
        }
        if (*str == '\0')
            break;

        str++;
    }
    return values;
}

int main()
{
    // Read a line from the terminal.
    // const size_t count = 100;
    // char line[count];
    // fgets(&line, count, stdin);

    // Parse the list.
    // list numbers = parse_list(line);
    list numbers = parse_list("  1 , 2   , 3 \n");

    // Remove and print each element.
    iterator iter = &numbers;
    while (!at_end(iter))
    {
        printf("%d\n", get(iter));
        remove_at(iter);
    }
}
