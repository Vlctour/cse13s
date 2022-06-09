#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "bv.h"

#define BYTE 8

// Create a struct for a bit vector.
struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// Creates a bit vector on the struct.
// Returns the pointer to the bit vector.
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv) {
        bv->vector = (uint8_t *) calloc((length + 7) / 8, sizeof(uint8_t));
        if (bv->vector) {
            bv->length = length;
            return bv;
        }
        free(bv);
        bv = NULL;
    }

    return bv;
}

// Prints out the bit vector
void bv_print(BitVector *bv) {
    uint32_t nums[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    uint32_t start[8] = { 0 };
    uint32_t temp = 0;
    for (uint32_t i = 0; i < (bv_length(bv) + 7) / 8; i++) {
        temp = bv->vector[i];
        printf("byte: %2" PRIu32 ", ", i);
        for (uint32_t j = 0; j < 8; j++) {
            if (nums[j] <= temp) {
                start[j] = 1;
                temp -= nums[j];
            }
        }
        printf("0x");
        for (uint32_t k = 0; k < 8; k++) {
            printf("%d", start[k]);
            start[k] = 0;
        }
        printf("\n");
    }
}

// Deletes a bit vector stored on the heap.
void bv_delete(BitVector **bv) {
    if (*bv) {
        free((*bv)->vector);
        (*bv)->vector = NULL;
        free(*bv);
        *bv = NULL;
    }

    return;
}

// Return the length of a bit vector.
uint32_t bv_length(BitVector *bv) {
    if (bv) {
        return bv->length;
    }

    return 0;
}

// Sets a bit in the bit vector.
// Returns true if successful, false if otherwise.
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (!bv || i < 0 || i >= bv_length(bv)) {
        return false;
    }

    bv->vector[i / BYTE] |= (0x1 << i % BYTE);

    return true;
}

// Clear a bit in the bit vector.
// Returns true if successful, false if otherwise.
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (!bv || i < 0 || i >= bv_length(bv)) {
        return false;
    }

    bv->vector[i / BYTE] &= ~(0x1 << i % BYTE);

    return true;
}

// Get a bit in the bit vector.
// Returns true if the value of the bit is 1, otherwise false.
bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (!bv || i < 0 || i >= bv_length(bv)) {
        return false;
    }

    uint8_t temp = bv->vector[i / BYTE];

    return temp >> i % BYTE & 0x1;
}
