#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Null for an empty list.
typedef struct list_node *list;

// list is a pointer to a struct list_node.
// if you have a list (list *nums), the first node is (*nums)
struct list_node
{
  int value;
  list tail;
};

// Appends a number to the end of a list.
void append(list *nums, int num)
{
  // Construct a new node to go at the end of the list.
  struct list_node new_node = {
      .value = num,
      .tail = NULL,
  };

  // Place the new node in dynamically allocated memory:
  list new_tail = malloc(sizeof(struct list_node));
  *new_tail = new_node;

  // Traverse the list until we get to the end.
  while (*nums != NULL)
    nums = &(*nums)->tail;

  // 'nums' now points to the last 'tail' slot, where we need to put the
  // pointer to the new node.
  *nums = new_tail;
}

// Removes the first element from the list, storing it in 'out'. Returns
// false (and leaves 'out' unmodified) if the list is empty.
bool remove_front(list *nums, int *out)
{
  // Check if the list is empty.
  if (*nums == NULL)
    return false;

  // Extract the first element.
  list old_front = *nums;
  *out = old_front->value;

  // Remove and free the first node.
  *nums = old_front->tail;
  free(old_front);
  return true;
}

// Removes the last element from the list, storing it in 'out'. Returns
// false (and leaves 'out' unmodified) if the list is empty.
bool remove_back(list *nums, int *out)
{
  if (*nums == NULL)
    return false;

  // remove front if there is only one element in the list
  if ((*nums)->tail == NULL)
    return remove_front(nums, out);

  // Traverse the list until we get to one before the end of the list.
  while ((*nums)->tail->tail != NULL)
    nums = &(*nums)->tail;

  // Extract the back value
  *out = (*nums)->tail->value;

  // Remove the last node
  (*nums)->tail = NULL;
  free((*nums)->tail);
  return true;
}

int main()
{
  // List is initially empty.
  list numbers = NULL;

  // Add numbers 1 through 10 to the list, in order.
  printf("Appending numbers 1 .. 10\n");
  for (int i = 1; i <= 10; i++)
    append(&numbers, i);

  int num;

  // Remove and print each element.
  printf("printing numbers 1 .. 10\n");
  while (remove_front(&numbers, &num))
    printf("%d\n", num);

  // Add numbers 1 through 10 to the list, in order.
  printf("Appending numbers 1 .. 10\n");
  for (int i = 1; i <= 10; i++)
    append(&numbers, i);

  // Remove and print each element in reverse
  printf("printing numbers 10 .. 1\n");
  while (remove_back(&numbers, &num))
    printf("%d\n", num);

  return 0;
}
