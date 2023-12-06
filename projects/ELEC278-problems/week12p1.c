#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

// Undirected Graph. No weights on edges. No self-edges.
struct graph
{
    struct vertex_node *vertices_head;
};

struct vertex_node
{
    int data;
    // head of the list for adge nodes.
    struct edge_node *edges_head;
    // next vertex in the overall graph list.
    struct vertex_node *next;
};

struct edge_node
{
    // Node that this edge connects to.
    struct vertex_node *other;
    // Next edge in the list of edges for a vertex.
    struct edge_node *next;
};

/**
 * Adds a vertex to the graph.
 *
 * @param g The graph to add the vertex to.
 * @param data The data to store in the vertex.
 * @return True if the vertex was added, false if it already existed.
 */
bool add_vertex(struct graph *g, int data)
{
    assert(g != NULL);
    // Allocate a new vertex node in g->vertices_head.

    // put this node at the end of the list stored in g.
    // this ensures that there are no duplicates.
    struct vertex_node **curr = &g->vertices_head;
    while (*curr != NULL)
    {
        if ((*curr)->data == data)
            return false;
        curr = &(*curr)->next;
    }

    *curr = malloc(sizeof(struct vertex_node));
    (**curr) = (struct vertex_node){data, NULL, NULL};
    return true;
}

/**
 * Adds an undirected edge between two vertices. Assume that verticies are never adjacent to themselves.
 *
 * @param g The graph to add the edge to.
 * @param v1 The first vertex.
 * @param v2 The second vertex.
 */
bool add_edge(struct graph *g, int v1, int v2)
{
    assert(g != NULL);
    if (v1 == v2)
        return false;
    struct vertex_node *v1_node = NULL, *v2_node = NULL;
    // Find the two nodes v1 and v2 in the graph g list.
    struct vertex_node *curr = g->vertices_head;
    // Upon reaching the end of this while loop, v1_node and v2_node should be set.
    while (v1_node == NULL || v2_node == NULL)
    {
        if (curr == NULL)
            return false;
        if (curr->data == v1)
            v1_node = curr;
        else if (curr->data == v2)
            v2_node = curr;
        curr = curr->next;
    }
    // Make sure that v1 and v2 are not already connected. if so, return false.
    // Start by traversing the list of edges for v1_node.
    struct edge_node *curr_edge = v1_node->edges_head;
    while (curr_edge != NULL)
    {
        // If the other node is v2_node, then they are already connected.
        if (curr_edge->other == v2_node)
            return false;
        curr_edge = curr_edge->next;
    }
    // Put v2_node at the head of the list of edges for v1_node.
    struct edge_node *new_edge = malloc(sizeof(struct edge_node));
    *new_edge = (struct edge_node){v2_node, v1_node->edges_head};
    v1_node->edges_head = new_edge;
    // Put v1_node at the head of the list of edges for v2_node.
    new_edge = malloc(sizeof(struct edge_node));
    *new_edge = (struct edge_node){v1_node, v2_node->edges_head};
    v2_node->edges_head = new_edge;

    return true;
}

/**
 * Determines whether two vertices are adjacent.
 *
 * @param g The graph to check.
 * @param v1 The first vertex.
 * @param v2 The second vertex.
 * @return True if the vertices are adjacent, false otherwise.
 */
bool are_adjacent(struct graph *g, int v1, int v2)
{
    assert(g != NULL);
    if (v1 == v2)
        return false;

    struct vertex_node *curr = g->vertices_head;
    // Look for v1 or v2 in the list of vertices, stop on whichever comes first.
    while (curr != NULL && curr->data != v1 && curr->data != v2)
        curr = curr->next;
    if (curr == NULL)
        return false;
    struct edge_node *curr_edge = curr->edges_head;
    if (curr->data == v1)
    {
        // Look for v2 in the list of edges for v1.
        while (curr_edge != NULL)
        {
            if (curr_edge->other->data == v2)
                return true;
            curr_edge = curr_edge->next;
        }
    }
    else if (curr->data == v2)
    {
        // Look for v1 in the list of edges for v2.
        while (curr_edge != NULL)
        {
            if (curr_edge->other->data == v1)
                return true;
            curr_edge = curr_edge->next;
        }
    }
    return false;
}

int main()
{
    struct graph g = {
        .vertices_head = NULL,
    };

    add_vertex(&g, 1);
    add_vertex(&g, 2);
    add_vertex(&g, 3);
    add_vertex(&g, 4);
    add_vertex(&g, 5);

    add_edge(&g, 1, 2);
    add_edge(&g, 1, 3);
    add_edge(&g, 1, 5);
    add_edge(&g, 2, 3);
    add_edge(&g, 3, 4);
    add_edge(&g, 3, 5);

    assert(are_adjacent(&g, 1, 3));
    assert(!are_adjacent(&g, 1, 4));

    return 0;
}
