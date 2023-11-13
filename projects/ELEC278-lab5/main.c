#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

// function to return -1 if it a < b
// returns 0 if it a == b
// returns 1 if a > b
int compare_int(int a, int b)
{
    return -(a < b) + (a > b);
}
// function to return -1 if it a < b
// returns 0 if it a == b
// returns 1 if a > b
int compare_ull(size_t a, size_t b)
{
    return -(a < b) + (a > b);
}

typedef struct _array
{
    // Pointer to your data
    void *data;
    // Number of bytes per element.
    size_t size;
    // Number of elements stored
    size_t length;
    // Number of elements memory has been allocated for.
    size_t capacity;
} array;

// Allocate new array. Returns NULL if memory could not be allocated, a valid pointer otherwise.
array *array_init(size_t element_size)
{
    array *a = malloc(sizeof(array));
    if (a == NULL)
        return a;
    a->data = NULL;
    a->size = element_size;
    a->capacity = a->length = 0;
    return a;
}

// Frees all data from the array and sets the a->data to NULL.
bool array_clear(array *a)
{
    assert(a != NULL);
    free(a->data);
    a->data = NULL;
    a->capacity = a->length = 0;
}

// Free array structure and all contained data.
bool array_free(array *a)
{
    assert(a != NULL);
    array_clear(a);
    free(a);
}

// Returns pointer to element at index i in the array. Index i must be less than a->length.
void *array_at(const array *a, size_t i)
{
    assert(a != NULL);
    assert(i < a->length);
    return a->data + i * a->size;
}

// Copy element located at value_ptr to the end of the array. Increases the capacity by double if more space is needed.
void array_append(array *a, void *value_ptr)
{
    switch (compare_ull(a->capacity, a->length))
    {
    // Array is full, must allocate more memory.
    case 0:
        // Set capacity to 1 if it was previously zero.
        a->capacity += (a->capacity == 0);
        // Re-allocate double the capacity.
        a->data = realloc(a->data, (a->size) * ((a->capacity) <<= 1));
    // Array has space. Just insert the new element.
    case 1:
        // Copy the memory from the value to be inserted, then increment length.
        memcpy(array_at(a, a->length++), value_ptr, a->size);
        break;
    default:
        assert(false);
        break;
    }
}

// Removes the last element from the array. Shrinks the capacity by half if the length is a quarter of the capacity.
void array_pop_back(array *a)
{
    switch (compare_ull(--a->length, a->capacity >> 2))
    {
    // if a->length <= a->capacity / 4
    case -1:
    case 0:
        // Re-allocate for half as much capacity.
        a->data = realloc(a->data, (a->size) * (a->capacity >>= 1));
    case 1:
        break;
    default:
        assert(false);
        break;
    }
}

// Returns pointer to the first element in the array.
void *array_front(const array *a)
{
    assert(a != NULL);
    assert(a->length > 0);
    return a->data;
}

// Returns pointer to the last element in the array.
void *array_back(const array *a)
{
    assert(a != NULL);
    assert(a->length > 0);
    return a->data + (a->length - 1) * a->size;
}

/* to create a dfs iterator for a binary tree, and to implement a next function:
    - you would have to create a bst_iterator struct that would contain a stack and the pointer to the current node.
        - each element in the stack would store a pointer to a node and a count.
        - initalize the iterator by putting the root node on the stack, with count = 1 (0: pre-order, 1:in-order, 2:post-order)
        - call next to set the iterator's current node
    - calling the next function on an iterator
        - check if stack is empty
            - if empty -> exit, returning false, indicating the end of iteration
        - pop an element from the stack
        - check if count on that element is zero
            - count == 0:
                - set the current node to be the new position of the iterator
                - pre-order:
                    - add left child node to the stack with count = 0
                    - add right child node to the stack with count = 0
                - in-order/post-order:
                    - do nothing
                - return true
            - count > 0:
                - subtract one from the count
                - next operation depends on type of traversal:
                    - pre-order: count == 0 always for pre-order, so you always process a node right away
                    - in-order:
                        - add right child node to the stack
                        - add current node to the stack
                        - add left child node to the stack
                    -
        - Unless that count is zero, the node is placed back on the stack, otherwise the iterator is set to it when next is called

    how it works:
        initializing iterator:
            // we have a stack
            stack s = stack_new();
            // put root node on the stack
            // BINARY TREE TRAVERSALS USE:: pre-order: pos = 2, in-order: pos = 1, post-order: pos = 0.
            stack_push(s, {root, pos});
            // call next for the first time to get the first node
            next(iter);
        calling next:
            // loop while stack is not empty
            while (!stack_empty(s))
            {
                // current node.
                bst node;
                // position for it to be added back on the stack.
                int pos;
                // get node and count from the stack. pretending pointers are being passed in here.
                stack_pop(s, &{node, pos});
                // set iterator to this node if it's count is -1.
                if (c == -1)
                {
                    iterator->current = node;
                    break;
                }
                // loop over child nodes, +1 to add back the parent node
                for (int i = 0, j = node->num_children - 1; i < node->num_children + 1; i++)
                {
                    // Push parent on stack with -1, so that it is processed next time.
                    if (i == c)
                    {
                        stack_push(s, {node, -1});
                        continue;
                    }
                    // for binary tree: left child is node->child[0], right child is node->child[1]
                    stack_push(s, {node->child[j--], pos});
                }
            }
*/
typedef struct bst_node *bst;
struct bst_node
{
    int value;
    bst left, right;
};

// Allocate a new bst_node at *position. Returns false if malloc did not work.
bool bst_new_node(bst *position, int value)
{
    assert(position != NULL);
    *position = malloc(sizeof(struct bst_node));
    if (*position == NULL)
        return false;
    (*position)->value = value;
    (*position)->left = NULL;
    (*position)->right = NULL;
    return true;
}

// Perform some function for each element in the binary search tree. Using a Pre-Order traversal.
void bst_foreach_preorder(bst tree, void (*func)(bst tree))
{
    if (tree != NULL)
    {
        (*func)(tree);
        bst_foreach_preorder(tree->left, func);
        bst_foreach_preorder(tree->right, func);
    }
}

// Perform some function for each element in the binary search tree. Using an In-Order traversal.
void bst_foreach_inorder(bst tree, void (*func)(bst tree))
{
    if (tree != NULL)
    {
        bst_foreach_inorder(tree->left, func);
        (*func)(tree);
        bst_foreach_inorder(tree->right, func);
    }
}

// Prints value in the tree node followed by a comma: "%d, ".
void bst_print_element(const bst tree)
{
    if (tree != NULL)
        printf("%d, ", tree->value);
}

// Prints the elements in a bst using an In-Order Traversal.
void bst_print(const bst tree)
{
    printf("{");
    bst_foreach_inorder(tree, &bst_print_element);
    printf("}\n");
}

// Returns a pointer to the position where a value is in the tree. If the value does not exist in the tree, returns a position where it could be inserted.
bst *find_position(bst *tree, int value)
{
    assert(tree != NULL);
    while (*tree != NULL)
    {
        switch (compare_int(value, (*tree)->value))
        {
        // value < (*tree)->value, search left subtree.
        case -1:
            tree = &(*tree)->left;
            break;
        // Equal. Return position.
        case 0:
            return tree;
            break;
        // (*tree)->value < value, search right subtree.
        case 1:
            tree = &(*tree)->right;
            break;
        default:
            assert(false);
            break;
        }
    }
    // Ended at a NULL node. This is the position where value could be inserted.
    return tree;
}

// Returns a pointer to the position of the maximum value in the tree. If the tree is empty, null is returned.
bst *find_max(bst *tree)
{
    assert(tree != NULL);
    if (*tree == NULL)
        return NULL;
    while ((*tree)->right != NULL)
        tree = &(*tree)->right;
    return tree;
}

// Returns a pointer to the position of the minimum value in the tree. If the tree is empty, null is returned.
bst *find_min(bst *tree)
{
    assert(tree != NULL);
    if (*tree == NULL)
        return NULL;
    while ((*tree)->left != NULL)
        tree = &(*tree)->left;
    return tree;
}

// Returns true if value was inserted successfully, false otherwise.
bool bst_insert(bst *tree, int value)
{
    assert(tree != NULL);
    // Find position where this value could be inserted.
    tree = find_position(tree, value);
    switch (*tree == NULL)
    {
    // Found a NULL position where value can be inserted.
    case true:
        assert(bst_new_node(tree, value));
        return true;
        break;
    // Position was not NULL, meaning that value was already in the tree.
    default:
        assert((*tree)->value == value);
        return false;
        break;
    }
}

// Returns true if value was removed successfully, false if the value was not present in the tree.
bool bst_remove(bst *tree, int value)
{
    assert(tree != NULL);
    // 3 cases, no children, 1 child, 2 children.

    // Find the value in the tree.
    tree = find_position(tree, value);
    bst temp = NULL;
    // Value not present in the tree, so it cannot be removed.
    if (*tree == NULL)
        return false;
    switch (-((*tree)->left != NULL) + 2 * ((*tree)->right != NULL))
    {
    // No children
    case 0:
        // Set node to Null.
        (*tree) = NULL;
        break;
    // Only have left child.
    case -1:
        // Swap with left child.
        temp = *tree;
        *tree = (*tree)->left;
        break;
    // Only have right child.
    case 2:
        // Swap with right child
        temp = *tree;
        *tree = (*tree)->right;
        break;
    // Both chlidren
    // Two options:
    //  >>> 1. swap with the maximum element in the left subtree
    //      2. swap with minimum element in right subtree.
    case 1:
        // temp is now the position of the maximum element in the left subtree.
        temp = *find_max(&(*tree)->left);
        (*tree)->value = temp->value;
        // Recursively remove the maximum element in the left subtree.
        bst_remove(&(*tree)->left, temp->value);
        break;
    default:
        assert(false);
        break;
    }
    free(temp);
    return true;
}

// Pointer to a tree. Used in bst_union and bst_intersection.
static bst *_temp_tree = NULL;
// Insert tree->value into temp_tree.
void temp_tree_insert(const bst tree)
{
    assert(_temp_tree != NULL);
    bst_insert(_temp_tree, tree->value);
}

// Inserts all elements from source into *desitination.
void bst_bst_insert_all_preorder(bst *destination, const bst source)
{
    _temp_tree = destination;
    bst_foreach_preorder(source, &temp_tree_insert);
}
// Returns a pointer to a tree containing all elements from both tree1 and tree2.
bst bst_union(const bst tree1, const bst tree2)
{
    bst new_tree = NULL;
    // Insert all elements from trees 1 and 2 into new_tree.
    bst_bst_insert_all_preorder(&new_tree, tree1);
    bst_bst_insert_all_preorder(&new_tree, tree2);
    return new_tree;
}

// Used in array_append_all_bst_inorder.
static array **_temp_arr = NULL;
// Current index in _temp_arr.
static size_t _temp_index;
void temp_arr_append_bst(const bst tree)
{
    assert(_temp_arr != NULL);
    array_append(*_temp_arr, &tree->value);
}
// Traverses source using an In-Order traversal, appending each element to the destination array.
void array_append_all_bst_inorder(array *destination, const bst source)
{
    _temp_arr = &destination;
    bst_foreach_inorder(source, &temp_arr_append_bst);
}

// Insert a value into the tree pointed to by _temp_tree if it finds a matching value in _temp_arr.
void bst_conditional_insert_temp_tree(const bst tree)
{
    // Increment through _temp_arr until tree->value <= *(int *)array_at(*_temp_arr, _temp_index).
    while (_temp_index + 1 < (*_temp_arr)->length && *(int *)array_at(*_temp_arr, _temp_index) < tree->value)
        ++_temp_index;
    // Values are equal, insert the value into _temp_tree
    if (*(int *)array_at(*_temp_arr, _temp_index) == tree->value)
        temp_tree_insert(tree);
    // If values were not equal, we had tree->value < *(int *)array_at(*_temp_arr, _temp_index).
    // In that case, just skip to the next element in the tree.
}

// Returns a bst tree containing all elements that are present in both tree1 and tree2.
bst bst_intersection(const bst tree1, const bst tree2)
{
    // Traverse tree1 In-Order, inserting all elements into an array.
    array *tree1_elements_inorder = array_init(sizeof(int));
    array_append_all_bst_inorder(tree1_elements_inorder, tree1);

    // Set the _temp_arr.
    _temp_index = 0;
    _temp_arr = &tree1_elements_inorder;
    // Set the _temp_tree
    bst new_tree = NULL;
    _temp_tree = &new_tree;

    // Traverse tree2 In-Order, at the same time iterate through tree1_elements_inorder.
    // increment _temp_index while curr_tree1 < curr_tree2
    // If the current element in tree2 equals the current element in tree1_elements_inorder, insert it into the new_tree. Otherwise go to the next element in tree2.
    bst_foreach_inorder(tree2, &bst_conditional_insert_temp_tree);

    // Free the data in tree1_elements_inorder.
    array_free(tree1_elements_inorder);
    return new_tree;
}

int main()
{
    // testing compare.
    assert(compare_int(1, 2) == -1);
    assert(compare_int(1, 1) == 0);
    assert(compare_int(1, -1) == 1);

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
    bst_print(tree2);

    // Should print: {0, 1, 2, 3, 5, 7, 9, 12, 13, 19, }
    bst union_1_2 = bst_union(tree1, tree2);
    bst_print(union_1_2);

    // Should print: {0, 1, 5, 7, 13, }
    bst intersection_1_2 = bst_intersection(tree1, tree2);
    bst_print(intersection_1_2);

    assert(bst_remove(&tree1, 1));
    assert(!bst_remove(&tree1, 12));
    assert(bst_remove(&tree1, 7));
    assert(bst_remove(&tree1, 9));
    assert(bst_remove(&tree1, 13));

    // Should print: {0, 2, 3, 5, }
    bst_print(tree1);

    return 0;
}
