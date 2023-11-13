#include <malloc.h>
#include <assert.h>
#include "stack.h"

struct stack_node
{
    // TODO (task 1): what fields do we need here?
    char data;
    struct stack_node *below;
};

struct stack
{
    // Null if the stack is empty.
    struct stack_node *head;
};

stack_ptr stack_new()
{
    stack_ptr s = malloc(sizeof(struct stack));
    s->head = NULL;
    return s;
}

void stack_free(stack_ptr s)
{
    // TODO (task 1): how do we make sure the nodes don't leak memory?
    assert(s != NULL);
    struct stack_node *temp;
    while (s->head != NULL)
    {
        // Copy the current top node.
        temp = s->head;
        // Make the second node the top node.
        s->head = s->head->below;
        free(temp);
    }
    free(s);
}

void stack_push(stack_ptr s, char c)
{
    // TODO (task 1): how do we push an entry onto the stack?
    assert(s != NULL);
    // allocate a new stack_node.
    struct stack_node *new_node = malloc(sizeof(struct stack_node));
    // put the new character in the stack.
    new_node->data = c;
    // make the new stack_node point to s->head.
    new_node->below = s->head;
    // make s->head be the new stack_node.
    s->head = new_node;
}

bool stack_pop(stack_ptr s, char *out)
{
    // TODO (task 1): how do we pop an entry from the stack?
    assert(s != NULL);
    // return false if the stack is empty.
    if (s->head == NULL)
        return false;
    // store the value at the head into out.
    *out = s->head->data;
    // store a temp variable to s->head.
    struct stack_node *removed = s->head;
    // make s->head be the s->head->below.
    s->head = s->head->below;
    // free the memory of the removed node.
    free(removed);
    return true;
}
