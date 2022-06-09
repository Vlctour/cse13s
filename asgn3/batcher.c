#include "batcher.h"

// Compares and swaps values for batcher sort.
// Code derived from asgn3.
// Takes in a struct stats, an array A, and 2 unsigned 32-bit intergers x and y.
void comparator(Stats *stats, uint32_t *A, uint32_t x, uint32_t y) {
    if (cmp(stats, A[x], A[y]) > 0) {
        swap(stats, &A[x], &A[y]);
    }
}

// Sorts a given array via a sorting algorithm called batcher sort.
// Code derived from asgn3.
// Takes in a struct stats, an array A, and the length of the array n.
void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t d, p, q, r, t = 0, temp = n;
    if (n == 0) {
        return;
    }
    while (temp > 0) {
        t += 1;
        temp = temp >> 1;
    }
    p = 1 << (t - 1);
    while (p > 0) {
        q = 1 << (t - 1);
        r = 0;
        d = p;
        while (d > 0) {
            for (uint32_t i = 0; i < (n - d); i++) {
                if ((i & p) == r) {
                    comparator(stats, A, i, i + d);
                }
            }
            d = q - p;
            q >>= 1;
            r = p;
        }
        p >>= 1;
    }
}
