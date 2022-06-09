#include <stdio.h>
#include <stdlib.h>
#include "pq.h"

// Create a struct called Priority Queue.
struct PriorityQueue {
    uint32_t head;
    uint32_t tail;
    uint32_t capacity;
    Node **nodes;
};

// Create a priority queue that can hold capacity number of nodes.
// Returns the pointer to the priority queue.
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *pq = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (pq != NULL) {
        pq->head = 0;
        pq->tail = 0;
        pq->capacity = capacity;
        pq->nodes = (Node **) malloc(pq->capacity * sizeof(Node *));
        if (pq->nodes == NULL) {
            free(pq);
            pq = NULL;
        }
    }

    return pq;
}

// Removes a priority queue from the heap.
// Sets the priority queue to point back at NULL.
void pq_delete(PriorityQueue **q) {
    if (*q != NULL) {
        free((*q)->nodes);
        (*q)->nodes = NULL;
        free(*q);
        *q = NULL;
    }

    return;
}

// Determines whether the priority queue is empty.
// Returns true if it's empty, otherwise false.
bool pq_empty(PriorityQueue *q) {
    if (q) {
        return q->head == q->tail;
    }

    return true;
}

// Determines whether the priority queue is full.
// Returns true if it's full, otherwise false.
bool pq_full(PriorityQueue *q) {
    if (q) {
        return ((q->head + 1) % q->capacity) == q->tail;
    }

    return true;
}

// Determines the size of the priority queue.
// Returns the number of nodes inside the priority queue.
uint32_t pq_size(PriorityQueue *q) {
    if (q) {
        return q->head - q->tail;
    }

    return 0;
}

// Given a priority queue and a node, find the correct spot to input the node.
// Got the insertion logic mostly from my asgn 3 insertion sort.
// Reading from left to right, order is highest priority to lowest priority.
void insertion_sort(PriorityQueue *q, Node *n) {
    uint32_t j = 0;
    while (true) {
        if (q->head == j) {
            break;
        }

        if (n->frequency <= q->nodes[j]->frequency) {
            break;
        }

        j += 1;
    }

    for (uint32_t i = pq_size(q); i > j; i--) {
        q->nodes[i] = q->nodes[i - 1];
    }

    q->nodes[j] = n;

    return;
}

// Enqueue a node into a priority queue.
// Uses a type of insertion sort to enqueue.
// Returns true if enqueue was successful, otherwise false.
bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q) || !q) {
        return false;
    }

    if (pq_empty(q)) {
        q->nodes[q->head] = n;
        q->head = (q->head + 1);
    } else {
        insertion_sort(q, n);
        q->head = (q->head + 1);
    }

    return true;
}

// Dequeues a node from the priority queue.
// Returns true if successful, otherwise false.
bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q) || !q) {
        return false;
    }

    *n = q->nodes[q->tail];
    for (uint32_t i = 0; i < pq_size(q) - 1; i++) {
        q->nodes[q->tail + i] = q->nodes[q->tail + i + 1];
    }

    q->head = (q->head - 1);

    return true;
}

// Print out all the nodes frequencies in the queue along with their position in the queue.
void pq_print(PriorityQueue *q) {
    printf("head: %u, tail: %u\n", q->head, q->tail);
    for (uint32_t i = 0; i < pq_size(q); i++) {
        printf("Node index: %u, node frequency:%lu\n", i, q->nodes[i]->frequency);
    }

    printf("\n");

    return;
}
