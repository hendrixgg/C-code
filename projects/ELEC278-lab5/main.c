#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct bst_node *bst;

struct bst_node
{
    int value;
    bst left, right;
};

// Prints left subtree elements, current tree element, then right subtree elements.
void bst_print_elements(bst tree)
{
    if (tree != NULL)
    {
        bst_print_elements(tree->left);
        printf("%d, ", tree->value);
        bst_print_elements(tree->right);
    }
}

void bst_print(bst tree)
{
    printf("{");
    bst_print_elements(tree);
    printf("}\n");
}

bool bst_insert(bst *tree, int value)
{
    // TODO: Task 1: insert 'value' into 'tree'.
    return false;
}

bool bst_remove(bst *tree, int value)
{
    // TODO: Task 2: remove 'value' from 'tree'.
    // 3 cases, no children, 1 child, 2 children.
    return false;
}

bst bst_union(bst tree1, bst tree2)
{
    // TODO: Task 3: compute the union of 'tree1' and 'tree2'.
    return NULL;
}

bst bst_intersection(bst tree1, bst tree2)
{
    // TODO: Task 3: compute the intersection of 'tree1' and 'tree2'.
    return NULL;
}

int main()
{
    // Initialize two trees.
    bst tree1 = NULL, tree2 = NULL;

    assert(bst_insert(&tree1, 1));
    assert(bst_insert(&tree1, 9));
    assert(bst_insert(&tree1, 2));
    assert(bst_insert(&tree1, 7));
    assert(bst_insert(&tree1, 0));
    assert(bst_insert(&tree1, 3));
    assert(bst_insert(&tree1, 13));
    assert(bst_insert(&tree1, 5));
    assert(!bst_insert(&tree1, 3));

    // Should print: {0, 1, 2, 3, 5, 7, 9, 13, }
    bst_print(tree1);

    assert(bst_insert(&tree2, 12));
    assert(bst_insert(&tree2, 13));
    assert(bst_insert(&tree2, 1));
    assert(bst_insert(&tree2, 0));
    assert(bst_insert(&tree2, 19));
    assert(bst_insert(&tree2, 7));
    assert(!bst_insert(&tree2, 13));
    assert(bst_insert(&tree2, 5));

    // Should print: {0, 1, 5, 7, 12, 13, }
    bst_print(tree1);

    // Should print: {0, 1, 2, 3, 5, 7, 9, 12, 13, 19, }
    bst union_1_2 = bst_union(tree1, tree2);
    bst_print(union_1_2);

    // Should print: {0, 1, 5, 7, 13, }
    bst intersection_1_2 = bst_intersection(tree1, tree2);
    bst_print(intersection_1_2);

    return 0;
}
