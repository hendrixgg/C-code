#include <stdio.h>
#include <stdlib.h>

#define M 3

// Null for an empty tree.
typedef struct tree_node *tree;

struct tree_node
{
    int value;
    tree children[M];
};

int sum(tree t)
{
    // Compute the sum of the elements in the tree.
    if (t == NULL)
        return 0;
    int return_sum = t->value;
    for (int i = 0; i < M; i++)
        return_sum += sum(t->children[i]);
    return return_sum;
}

int main()
{
    struct tree_node child1 = {
        .value = 1,
        .children = {NULL, NULL, NULL},
    };
    struct tree_node child2 = {
        .value = 2,
        .children = {NULL, NULL, NULL},
    };
    struct tree_node child3 = {
        .value = 3,
        .children = {NULL, NULL, NULL},
    };
    struct tree_node root =
        {
            .value = 10,
            .children = {&child1, &child2, &child3},
        };

    printf("%d\n", sum(&root));
    return 0;
}
