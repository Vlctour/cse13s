#include <stdio.h>
#include <stdlib.h>
#include "node.h"

// Creates a node on the heap.
// Returns the pointer to the node
Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *node = (Node *) malloc(sizeof(Node));
    if (node != NULL) {
        node->left = NULL;
        node->right = NULL;
        node->symbol = symbol;
        node->frequency = frequency;
    }

    return node;
}

// Deletes the node on the heap.
void node_delete(Node **n) {
    if (*n != NULL) {
        free(*n);
        *n = NULL;
    }

    return;
}

// Creates a parent node by combining the left and right nodes.
// Returns a pointer to the parent node.
Node *node_join(Node *left, Node *right) {
    uint64_t frequency = left->frequency + right->frequency;
    uint8_t symbol = '$';
    Node *parent = node_create(symbol, frequency);
    if (parent) {
        parent->left = left;
        parent->right = right;
    }
    return parent;
}

// Prints out the state of a node.
void node_print(Node *n) {
    printf("Symbol: %c\n", n->symbol);
    printf("Frequency: %lu\n", n->frequency);

    if (n->left) {
        printf("Left symbol: %c\n", n->left->symbol);
        printf("Left frequency: %lu\n", n->left->frequency);
    }

    if (n->right) {
        printf("Right symbol: %c\n", n->right->symbol);
        printf("Right frequency: %lu\n", n->right->frequency);
    }

    printf("\n");

    return;
}
