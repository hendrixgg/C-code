#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
    assert(iter != NULL);
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
    assert(iter != NULL);
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
    assert(iter != NULL);
    return *iter == NULL;
}

// Gets the value at an iterator.
// Only valid if !at_end(iter).
int get(iterator iter)
{
    assert(iter != NULL);
    return (*iter)->value;
}

// Advances an iterator to the next position.
// Returns false if the iterator is at the end of the list.
bool next(iterator *iter)
{
    assert(iter != NULL);
    if (at_end(*iter))
        return false;

    assert(*iter != NULL);
    *iter = &(**iter)->tail;
    return true;
}

// Prints the contents of the list.
// use this formatting: 1, 2, 3, 3, 4, 5, 6
void print_list(list nums)
{
    iterator iter = &nums;

    // Check if the list is empty.
    if (at_end(iter))
        printf("<empty list>");

    // Loop through the list and print each value.
    while (!at_end(iter) && !at_end(&(*iter)->tail))
    {
        printf("%d, ", get(iter));
        next(&iter);
    }
    printf("%d\n", get(iter));
}

// Swaps adjacent items in the list.
void swap_adjacent(list *nums)
{
    iterator iter = nums;

    while (!at_end(iter) && !at_end(&(*iter)->tail))
    {
        // Swap the values of adjacent nodes.
        int temp = get(iter);
        (*iter)->value = get(&(*iter)->tail);
        (*iter)->tail->value = temp;

        // Move the iterator to the next pair of nodes.
        next(&iter);
        next(&iter);
    }
}

// Duplicates the list, appending the duplicate list to the end.
void double_list(iterator nums)
{
    assert(nums != NULL);
    assert(*nums != NULL);

    // traverse nums, creating a duplicate list.
    list duplicate_nums = NULL;
    iterator iter1 = nums, iter2 = &duplicate_nums;
    while (!at_end(&(*iter1)->tail))
    {
        insert_at(iter2, get(iter1));
        next(&iter1);
        next(&iter2);
    }
    insert_at(iter2, get(iter1));
    // append the duplicate list to the end of nums.
    (*iter1)->tail = duplicate_nums;
}

// Removes adjacent duplicate items in the list.
void remove_adjacent_duplicates(list *nums)
{
    iterator iter = nums;

    while (!at_end(iter) && !at_end(&(*iter)->tail))
    {
        if (get(iter) == get(&(*iter)->tail))
        {
            // Remove the duplicate node.
            remove_at(iter);
        }
        else
        {
            // Move the iterator to the next node.
            next(&iter);
        }
    }
}

int main()
{
    // List is initially empty.
    list numbers = NULL;

    // construct a linked list containing the items 1, 2, 3, 3, 4, 5, 6
    insert_at(&numbers, 6);
    insert_at(&numbers, 5);
    insert_at(&numbers, 4);
    insert_at(&numbers, 3);
    insert_at(&numbers, 3);
    insert_at(&numbers, 2);
    insert_at(&numbers, 1);

    printf("Original list: ");
    print_list(numbers);

    printf("Swapping adjacent items: ");
    swap_adjacent(&numbers);
    print_list(numbers);

    printf("Duplicating all items: ");
    double_list(&numbers);
    print_list(numbers);

    printf("Removing adjacent duplicates: ");
    remove_adjacent_duplicates(&numbers);
    print_list(numbers);

    return 0;
}
