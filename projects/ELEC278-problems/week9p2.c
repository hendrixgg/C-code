#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Null for an empty tree.
typedef struct avl_node *avl_tree;

struct avl_node
{
    int value;
    enum
    {
        LEFT_UNBALANCED = -2,
        LEFT_HEAVY = -1,
        BALANCED = 0,
        RIGHT_HEAVY = 1,
        RIGHT_UNBALANCED = 2,
    } bf;
    avl_tree left, right;
};

void avl_rotate_left(avl_tree *subtree)
{
    assert((*subtree)->right != NULL);
    avl_tree middle = (*subtree)->right->left;
    (*subtree)->right->left = *subtree;
    *subtree = (*subtree)->right;
    (*subtree)->left->right = middle;
}

void avl_rotate_right(avl_tree *subtree)
{
    assert((*subtree)->left != NULL);
    avl_tree middle = (*subtree)->left->right;
    (*subtree)->left->right = *subtree;
    *subtree = (*subtree)->left;
    (*subtree)->right->left = middle;
}

bool avl_insert(avl_tree *tree, int value)
{
    struct stack_node
    {
        avl_tree *location;
        bool on_right;
        struct stack_node *tail;
    } *stack = NULL;
    bool result = false;

    while (*tree != NULL)
    {
        if ((*tree)->value == value)
            goto cleanup;

        bool on_right = value > (*tree)->value;

        struct stack_node *new_node = malloc(sizeof(struct stack_node));
        new_node->location = tree;
        new_node->on_right = on_right;
        new_node->tail = stack;
        stack = new_node;

        tree = on_right ? &(*tree)->right : &(*tree)->left;
    }

    result = true;

    *tree = malloc(sizeof(struct avl_node));
    (*tree)->value = value;
    (*tree)->bf = BALANCED;
    (*tree)->left = NULL;
    (*tree)->right = NULL;

    while (stack != NULL)
    {
        avl_tree *curr = stack->location;
        if (stack->on_right)
            (*curr)->bf++;
        else
            (*curr)->bf--;

        struct stack_node *tail;
        switch ((*curr)->bf)
        {
        case BALANCED:
            goto cleanup;
        case LEFT_UNBALANCED:
            if ((*curr)->left->bf == RIGHT_HEAVY)
            {
                avl_rotate_left(&(*curr)->left);
                (*curr)->left->left->bf = (*curr)->left->bf == RIGHT_HEAVY ? LEFT_HEAVY : BALANCED;
                (*curr)->left->bf = (*curr)->left->bf == LEFT_HEAVY ? LEFT_UNBALANCED : LEFT_HEAVY;
            }
            avl_rotate_right(curr);
            (*curr)->right->bf = (*curr)->bf == LEFT_UNBALANCED ? RIGHT_HEAVY : BALANCED;
            (*curr)->bf = BALANCED;
            goto cleanup;
        case RIGHT_UNBALANCED:
            if ((*curr)->right->bf == LEFT_HEAVY)
            {
                avl_rotate_right(&(*curr)->right);
                (*curr)->right->right->bf = (*curr)->right->bf == LEFT_HEAVY ? RIGHT_HEAVY : BALANCED;
                (*curr)->right->bf = (*curr)->right->bf == RIGHT_HEAVY ? RIGHT_UNBALANCED : RIGHT_HEAVY;
            }
            avl_rotate_left(curr);
            (*curr)->left->bf = (*curr)->bf == RIGHT_UNBALANCED ? LEFT_HEAVY : BALANCED;
            (*curr)->bf = BALANCED;
            goto cleanup;
        default:
            tail = stack->tail;
            free(stack);
            stack = tail;
            continue;
        }
    }

cleanup:
    while (stack != NULL)
    {
        struct stack_node *tail = stack->tail;
        free(stack);
        stack = tail;
    }
    return result;
}

void avl_print_set(avl_tree tree)
{
    if (tree != NULL)
    {
        avl_print_set(tree->left);
        printf("%d, ", tree->value);
        avl_print_set(tree->right);
    }
}

// Time Complexity: O(n*log(n)) with n = (number of nodes in source).
void avl_filter_copy(avl_tree *destination, avl_tree source, int x)
{
    if (source == NULL)
        return;

    if (source->value >= x)
    {
        // Insert the value since it is greater than x.
        avl_insert(destination, source->value);
        // Insert the valid values present in the left subtree.
        avl_filter_copy(destination, source->left, x);
    }
    // Insert the valid values present in the right subtree.
    avl_filter_copy(destination, source->right, x);
}

// Could be done better if we find the right subtree containing all values >= x, copy that subtree, then insert values >=x that are in the left subtree.
avl_tree avl_filter(avl_tree t, int x)
{
    // Construct a tree which contains the elements of t which are greater than or equal to x.
    avl_tree new_tree = NULL;
    // call recursive function to
    avl_filter_copy(&new_tree, t, x);
    return new_tree;
}

int main()
{
    avl_tree tree = NULL;
    for (int i = 0; i < 20; i++)
        avl_insert(&tree, i);

    printf("input: ");
    avl_print_set(tree);
    printf("\n");

    avl_tree subset = avl_filter(tree, 11);
    printf("output: ");
    avl_print_set(subset);
    printf("\n");
    return 0;
}
