#include <stdio.h>
#include <string.h>

struct student
{
    char name[50];
    int group;
    float marks;
};

void newstu(struct student *st_obj)
{
    strcpy(st_obj->name, "David");
    st_obj->group = 11;
    st_obj->marks = 10;
}

struct temp
{
    int a;
    int b;
    int c;
};

typedef struct node
{
    int value;
    struct node *next;
} Node;

Node *move_to_front(Node *head)
{
    Node *p, *q;
    if ((head == NULL) || (head->next == NULL))
        return head;

    q = NULL;
    p = head;
    while (p->next != NULL)
    {
        q = p;
        p = p->next;
    }
    q = NULL;
    p->next = head;
    head = p;

    // head = p;
    // p->next = q;
    // q = NULL;

    // q->next = NULL;
    // head = p;
    // p->next = head;

    // q->next = NULL;
    // p->next = head;
    // head = p;
    return head;
}

int main()
{
    // struct student st1 = {"Ian", 3, 6.5};
    // printf("Name: %s\n", st1.name);
    // printf("Roll: %d\n", st1.group);
    // newstu(&st1);
    // printf("Marks: %f\n", st1.marks);

    // struct temp p[] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    // printf("a: %d\n", p[1].b);

    Node n3 = {3, NULL};
    Node n2 = {2, &n3};
    Node n1 = {1, &n2};

    printf("%d, %d, %d\n", n1.value, n1.next->value, n1.next->next->value);

    move_to_front(&n1);

    printf("%d, %d, %d\n", n1.value, n1.next->value, n1.next->next->value);
    return 0;
}