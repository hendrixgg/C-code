#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Null for an empty tree.
typedef struct bst_node *bst;

struct bst_node
{
    int value;
    bst left, right;
};

bool bst_insert(bst *tree, int value)
{
    if (*tree == NULL)
    {
        *tree = malloc(sizeof(struct bst_node));
        (*tree)->value = value;
        (*tree)->left = NULL;
        (*tree)->right = NULL;
        return true;
    }
    else if (value < (*tree)->value)
        return bst_insert(&(*tree)->left, value);
    else if (value > (*tree)->value)
        return bst_insert(&(*tree)->right, value);
    else
        return false;
}

void bst_print_set(bst tree)
{
    if (tree != NULL)
    {
        bst_print_set(tree->left);
        printf("%d, ", tree->value);
        bst_print_set(tree->right);
    }
}

bst bst_filter(bst t, int x)
{
    // Construct a tree which contains the elements of t which are greater than or equal to x.
    if (t == NULL)
        return NULL;

    // We can insert the value of this node into the new_tree.
    if (t->value >= x)
    {
        bst new_tree = NULL;

        // Insert t->value.
        bst_insert(&new_tree, t->value);
        // insert the elements that are greater than t->value.
        new_tree->right = bst_filter(t->right, x);
        // insert the elements that are less than t->value and still greater than x.
        new_tree->left = bst_filter(t->left, x);

        return new_tree;
    }

    // x value is too small, move to the right for larger values.
    return bst_filter(t->right, x);
}

int main()
{
    bst tree = NULL;
    for (int i = 0; i < 20; i++)
        bst_insert(&tree, i * 3 % 20);

    printf("input: ");
    bst_print_set(tree);
    printf("\n");

    bst subset = bst_filter(tree, 11);
    printf("output: ");
    bst_print_set(subset);
    printf("\n");
    return 0;
}
