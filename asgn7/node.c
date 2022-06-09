#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

// Create a node on the heap. Store a string in the node along with the occurrences.
// Return the pointer to the node.
Node *node_create(char *word) {
    Node *node = (Node *) malloc(sizeof(Node));
    if (node) {
        node->word = strdup(word);
        if (node->word) {
            node->count = 0;
            return node;
        }

        free(node);
        node = NULL;
    }

    return node;
}

// Delete the node stored on the heap.
void node_delete(Node **n) {
    if (*n) {
        free((*n)->word);
        (*n)->word = NULL;
        free(*n);
        *n = NULL;
    }

    return;
}

// Print out data about a node.
void node_print(Node *n) {
    printf("String is: %s, count: %d\n", n->word, n->count);
    return;
}
