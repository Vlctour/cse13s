#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// Make a struct stack.
struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

// Create the stack on the heap.
// Returns the pointer to the stack.
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s != NULL) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (Node **) malloc(s->capacity * sizeof(Node *));
        if (s->items == NULL) {
            free(s);
            s = NULL;
        }
    }

    return s;
}

// Deletes the stack on the heap.
void stack_delete(Stack **s) {
    if (*s != NULL) {
        free((*s)->items);
        (*s)->items = NULL;
        free(*s);
        *s = NULL;
    }

    return;
}

// Check if stack is empty.
// Return true if it is, false otherwise.
bool stack_empty(Stack *s) {
    if (s) {
        return s->top <= 0;
    }

    return true;
}

// Check if stack is full.
// Return true if it is, false otherwise.
bool stack_full(Stack *s) {
    if (s) {
        return s->top == s->capacity;
    }

    return true;
}

// Returns the size of the stack.
uint32_t stack_size(Stack *s) {
    if (s) {
        return s->top;
    }

    return 0;
}

// Pushes a node to the top of the stack.
// Returns true if successful, false otherwise.
bool stack_push(Stack *s, Node *n) {
    if (stack_full(s)) {
        return false;
    }
    s->items[s->top] = n;
    s->top += 1;

    return true;
}

// Pops a node off the stack and puts it in the argument node.
// Returns true if successful, false otherwise.
bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s)) {
        return false;
    }
    s->top -= 1;
    *n = s->items[s->top];

    return true;
}

// Prints out all the nodes in the stack.
void stack_print(Stack *s) {
    for (uint32_t i = 0; i < stack_size(s); i++) {
        printf("index: %u, symbol %c, ", i, s->items[i]->symbol);
        printf("frequency: %lu\n", s->items[i]->frequency);
    }
    printf("\n");

    return;
}
