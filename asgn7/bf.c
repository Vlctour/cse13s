#include <stdlib.h>
#include "bf.h"
#include "bv.h"
#include "salts.h"
#include "speck.h"

// Make a struct for the bloom filter.
struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

// Make a bloom filter on the heap.
// Return the pointer to the bloom filter.
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        bf->filter = bv_create(size);
        if (bf->filter) {
            bf->primary[0] = SALT_PRIMARY_LO;
            bf->primary[1] = SALT_PRIMARY_HI;
            bf->secondary[0] = SALT_SECONDARY_LO;
            bf->secondary[1] = SALT_SECONDARY_HI;
            bf->tertiary[0] = SALT_TERTIARY_LO;
            bf->tertiary[1] = SALT_TERTIARY_HI;
            return bf;
        }

        free(bf);
        bf = NULL;
    }

    return bf;
}

// Deletes the bloom filtered stored on the heap.
void bf_delete(BloomFilter **bf) {
    if (*bf) {
        bv_delete(&(*bf)->filter);
        free(*bf);
        *bf = NULL;
    }
    return;
}

// Returns the number of accessible bits in the bloom filter.
uint32_t bf_size(BloomFilter *bf) {
    if (bf) {
        return bv_length(bf->filter);
    }

    return 0;
}

// Inserts a word into 3 different locations on the bloom filter according to the three
// hash ranges.
void bf_insert(BloomFilter *bf, char *word) {
    if (bf) {
        bv_set_bit(bf->filter, hash(bf->primary, word) % bf_size(bf));
        bv_set_bit(bf->filter, hash(bf->secondary, word) % bf_size(bf));
        bv_set_bit(bf->filter, hash(bf->tertiary, word) % bf_size(bf));
    }

    return;
}

// Checks to see if a word is in a bloom filter (not definitive).
// Returns true if it is, false otherwise.
bool bf_probe(BloomFilter *bf, char *word) {
    bool pri, sec, ter;
    if (bf) {
        pri = bv_get_bit(bf->filter, hash(bf->primary, word) % bf_size(bf));
        sec = bv_get_bit(bf->filter, hash(bf->secondary, word) % bf_size(bf));
        ter = bv_get_bit(bf->filter, hash(bf->tertiary, word) % bf_size(bf));
        if (pri && sec && ter) {
            return true;
        }
    }

    return false;
}

// Prints out the bloom filters' bit vector.
void bf_print(BloomFilter *bf) {
    if (bf) {
        bv_print(bf->filter);
    }

    return;
}
