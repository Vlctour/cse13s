#include <stdlib.h>
#include "randstate.h"

// Initialize global variable state.
gmp_randstate_t state;

// Initialize the state so we can generate random numbers in the gmp library.
// Takes in a seed.
//
// Returns nothing.
void randstate_init(uint64_t seed) {
    srandom(seed);
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
}

// Clear the memory used by state.
//
// Returns nothing.
void randstate_clear(void) {
    gmp_randclear(state);
}
