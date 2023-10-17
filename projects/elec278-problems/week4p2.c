#include <stdbool.h>
#include <stdio.h>

// The size of the arrays at each node.
#define ARRAY_SIZE 4

// A linked list, where each node contains an array; null for an empty list.
typedef struct list_node *hybrid_list;

struct list_node {
    int values[ARRAY_SIZE];
    hybrid_list tail;
};

struct queue {
    // TODO: what fields do we need?
};

void push(struct queue *s, int value) {
    // TODO: implement this.
}

bool pop(struct queue *s, int *out) {
    // TODO: implement this.
    return false;
}

int main() {
    struct queue s = {
            // TODO: initialize all fields.
    };

    for (int i = 1; i <= 10; i++) {
        printf("push: %d\n", i);
        push(&s, i);
    }

    int value;
    while (pop(&s, &value))
        printf("pop: %d", value);

    return 0;
}
