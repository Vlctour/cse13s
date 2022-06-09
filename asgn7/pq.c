#include <stdio.h>
#include <stdlib.h>
#include "pq.h"

// Create a struct called Author.
// Will contain an authors name and the distance.
struct Author {
    char *name;
    double dist_num;
};

typedef struct Author Author;

// Create a struct called Priority Queue.
struct PriorityQueue {
    uint32_t head;
    uint32_t tail;
    uint32_t capacity;
    Author **authors;
};

// Create a priority queue that can hold capacity number of authors.
// Returns the pointer to the priority queue.
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *pq = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (pq != NULL) {
        pq->head = 0;
        pq->tail = 0;
        pq->capacity = capacity;
        pq->authors = (Author **) malloc(capacity * sizeof(Author *));
        if (pq->authors != NULL) {
            for (uint32_t i = 0; i < capacity; i++) {
                pq->authors[i] = (Author *) malloc(sizeof(Author));
                pq->authors[i]->name = NULL;
                pq->authors[i]->dist_num = 0;
            }
            return pq;
        }
        free(pq);
        pq = NULL;
    }

    return pq;
}

// Removes a priority queue from the heap.
// Sets the priority queue to point back at NULL.
void pq_delete(PriorityQueue **q) {
    if (*q != NULL) {
        for (uint32_t i = 0; i < (*q)->capacity; i++) {
            free((*q)->authors[i]->name);
            (*q)->authors[i]->name = NULL;
            free((*q)->authors[i]);
            (*q)->authors[i] = NULL;
        }
        free((*q)->authors);
        (*q)->authors = NULL;
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

// Figures out the minimum child for a given parent node.
// Returns the smaller child.
uint32_t min_child(PriorityQueue *q, uint32_t first, uint32_t last) {
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if (right <= last && q->authors[right - 1]->dist_num <= q->authors[left - 1]->dist_num) {
        return right;
    }
    return left;
}

// Fixes the priority queue so that the next time we dequeue, we dequeue the lower child.
void fix_heap(PriorityQueue *q, uint32_t first, uint32_t last) {
    uint32_t found = 0, mother = first, small = min_child(q, mother, last);
    while ((mother <= (pq_size(q) / 2)) && (found == 0)) {
        if (q->authors[mother - 1]->dist_num >= q->authors[small - 1]->dist_num) {
            // Perform a swap
            Author temp = { "temp", 0.0 };
            temp.name = q->authors[mother - 1]->name;
            temp.dist_num = q->authors[mother - 1]->dist_num;
            q->authors[mother - 1]->name = q->authors[small - 1]->name;
            q->authors[mother - 1]->dist_num = q->authors[small - 1]->dist_num;
            q->authors[small - 1]->name = temp.name;
            q->authors[small - 1]->dist_num = temp.dist_num;

            mother = small;
            small = min_child(q, mother, last);
        } else {
            found = 1;
        }
    }

    return;
}

// Builds a heap.
void build_heap(PriorityQueue *q, uint32_t first, uint32_t last) {
    for (uint32_t father = last / 2; father != first - 1; father -= 1) {
        fix_heap(q, father, last);
    }

    return;
}

// Enqueue a author struct into a priority queue.
// Uses a type of heap sort to enqueue.
// Returns true if enqueue was successful, otherwise false.
bool enqueue(PriorityQueue *q, char *author, double dist) {
    if (pq_full(q) || !q) {
        return false;
    }
    q->authors[q->head]->name = author;
    q->authors[q->head]->dist_num = dist;
    q->head = (q->head + 1);

    return true;
}

// Dequeues the smallest distance/author pair in the priority queue.
// Returns true if it was successful, false otherwise.
bool dequeue(PriorityQueue *q, char **author, double *dist) {
    if (pq_empty(q) || !q) {
        return false;
    }
    build_heap(q, q->tail + 1, q->head);

    // Swap
    Author temp = { "temp", 0.0 };
    temp.name = q->authors[q->tail]->name;
    temp.dist_num = q->authors[q->tail]->dist_num;
    q->authors[q->tail]->name = q->authors[q->head - 1]->name;
    q->authors[q->tail]->dist_num = q->authors[q->head - 1]->dist_num;
    q->authors[q->head - 1]->name = temp.name;
    q->authors[q->head - 1]->dist_num = temp.dist_num;

    // Pops the author
    *author = q->authors[q->head - 1]->name;
    *dist = q->authors[q->head - 1]->dist_num;

    // Fix heap
    q->head = (q->head - 1);
    fix_heap(q, q->tail + 1, q->head);

    return true;
}

// Print out all the nodes frequencies in the queue along with their position in the queue.
void pq_print(PriorityQueue *q) {
    printf("head: %u, tail: %u, size: %d\n", q->head, q->tail, pq_size(q));
    for (uint32_t i = 0; i < pq_size(q); i++) {
        printf("A: %s, d: %f\n", q->authors[i]->name, q->authors[i]->dist_num);
    }

    printf("\n");

    return;
}
