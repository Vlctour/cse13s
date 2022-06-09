#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "code.h"

#define BYTE 8

// Create a code on the stack.
// Returns the pointer to the code.
Code code_init(void) {
    Code c[MAX_CODE_SIZE];
    c->top = 0;
    for (uint32_t i = 0; i < MAX_CODE_SIZE; i++) {
        c->bits[i] = 0x0;
    }

    return *c;
}

// Return the size of the code.
uint32_t code_size(Code *c) {
    if (c) {
        return c->top;
    }

    return 0;
}

// Returns true if code is empty, else false.
bool code_empty(Code *c) {
    if (c) {
        return c->top <= 0;
    }

    return true;
}

// Returns true if code is full, else false.
bool code_full(Code *c) {
    if (c) {
        return c->top == ALPHABET;
    }

    return true;
}

// Sets a bit at a specific index in the code.
// Returns true if successful, else false.
bool code_set_bit(Code *c, uint32_t i) {
    if (i < 0 || i > ALPHABET) {
        return false;
    }

    c->bits[i / BYTE] |= (0x1 << i % BYTE);

    return true;
}

// Clears a bit at a specific index in the code.
// Returns true if successful, else false.
bool code_clr_bit(Code *c, uint32_t i) {
    if (i < 0 || i > ALPHABET) {
        return false;
    }

    c->bits[i / BYTE] &= ~(0x1 << i % BYTE);

    return true;
}

// Get a bit at a specific index.
// If the bit is 1 return true, otherwise return false (due to either being 0 or out of range).
bool code_get_bit(Code *c, uint32_t i) {
    if (i < 0 || i > ALPHABET) {
        return false;
    }

    uint8_t temp = c->bits[i / BYTE];

    return temp >> i % BYTE & 0x1;
}

// Pushes a bit at the top of the code.
// Returns true if successful, otherwise false.
bool code_push_bit(Code *c, uint8_t bit) {
    if (code_full(c)) {
        return false;
    }

    if (bit == 1) {
        code_set_bit(c, c->top);
        c->top += 1;
        return true;
    } else if (bit == 0) {
        c->top += 1;
        return true;
    }

    return false;
}

// Pops a bit at the top of the code.
// Store popped bit into bit.
// Return true if successful, otherwise false.
bool code_pop_bit(Code *c, uint8_t *bit) {
    if (code_empty(c)) {
        return false;
    }

    c->top -= 1;
    if (code_get_bit(c, c->top)) {
        code_clr_bit(c, c->top);
        *bit = 1;
    } else {
        code_clr_bit(c, c->top);
        *bit = 0;
    }

    return true;
}

// Prints out the code in terms of binary.
void code_print(Code *c) {
    uint32_t nums[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    uint32_t sta[8] = { 0 };
    uint32_t temp = 0;
    for (uint32_t i = 0; i < MAX_CODE_SIZE; i++) {
        temp = c->bits[i];
        printf("byte: %2" PRIu32 ", ", i);
        for (uint32_t j = 0; j < 8; j++) {
            if (nums[j] <= temp) {
                sta[j] = 1;
                temp -= nums[j];
            }
        }
        printf("0x");
        for (uint32_t k = 0; k < 8; k++) {
            printf("%d", sta[k]);
            sta[k] = 0;
        }
        printf("\n");
    }
}
