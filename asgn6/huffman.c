#include "huffman.h"
#include "pq.h"
#include "io.h"
#include "stack.h"

// Initialize global index and buffer, used for dump tree.
static uint64_t ind;
static uint8_t abuffer[MAX_TREE_SIZE];

// Helper function for build code.
// Does the actual build code.
void helper_build_code(Node *root, Code table[static ALPHABET], Code c) {
    uint8_t temp_bit;
    if (root) {
        if (!(root->left) && !(root->right)) {
            table[root->symbol] = c;
        } else {
            code_push_bit(&c, 0);
            helper_build_code(root->left, table, c);
            code_pop_bit(&c, &temp_bit);

            code_push_bit(&c, 1);
            helper_build_code(root->right, table, c);
            code_pop_bit(&c, &temp_bit);
        }
    }

    return;
}

// Helper function for dump tree.
// Does the actual dump tree code.
void helper_dump_tree(int outfile, Node *root) {
    if (root) {
        helper_dump_tree(outfile, root->left);
        helper_dump_tree(outfile, root->right);
        if (!(root->left) && !(root->right)) {
            abuffer[ind] = 'L';
            ind += 1;
            abuffer[ind] = root->symbol;
            ind += 1;
        } else {
            abuffer[ind] = 'I';
            ind += 1;
        }
    }

    return;
}

// Build a tree given a histogram
// Return the root node of the tree
Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *q = pq_create(ALPHABET + 1);
    Node *temp_l;
    Node *temp_r;

    for (uint64_t i = 0; i < ALPHABET; i++) {
        if (hist[i] > 0) {
            Node *(tempi) = node_create(i, hist[i]);
            enqueue(q, tempi);
        }
    }
    while (pq_size(q) >= 2) {
        dequeue(q, &temp_l);
        dequeue(q, &temp_r);
        Node *parent = node_join(temp_l, temp_r);
        enqueue(q, parent);
    }

    Node *root;
    dequeue(q, &root);
    pq_delete(&q);

    return root;
}

// Given a code table, write the corresponding code to the outfile.
// Fill up a buffer and then write it to the outfile.
void build_codes(Node *root, Code table[static ALPHABET]) {
    static Code c;
    helper_build_code(root, table, c);

    return;
}

// Given the root node of a tree, write out the tree into the outfile.
// Should fill a buffer and then proceed to write out the data in BLOCK sizes.
void dump_tree(int outfile, Node *root) {
    helper_dump_tree(outfile, root);
    write_bytes(outfile, abuffer, ind);

    return;
}

// Rebuilds a tree given the dumped tree and size of the tree.
// Returns the root node of the constructed tree.
Node *rebuild_tree(uint16_t nbytes, uint8_t tree_dump[static nbytes]) {
    Stack *stack = stack_create(ALPHABET + 1);
    Node *root;
    Node *right;
    Node *left;
    for (uint16_t i = 0; i < nbytes; i++) {
        if (tree_dump[i] == 'I') {
            stack_pop(stack, &right);
            stack_pop(stack, &left);
            Node *parent = node_join(left, right);
            stack_push(stack, parent);
        }

        if (tree_dump[i] == 'L') {
            i++;
            Node *(tempi) = node_create(tree_dump[i], 0);
            stack_push(stack, tempi);
        }
    }
    stack_pop(stack, &root);
    stack_delete(&stack);

    return root;
}

// Given a root of a tree, delete the nodes used.
void delete_tree(Node **root) {
    if (*root) {
        delete_tree(&(*root)->left);
        delete_tree(&(*root)->right);
        if (!((*root)->left) && !((*root)->right)) {
            node_delete(root);
        }
    }

    return;
}
