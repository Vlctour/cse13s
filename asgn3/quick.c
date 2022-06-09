#include "quick.h"

// Code derived from asgn3.
// Takes in a struct stats, an array A, and 2 unsigned 32-bit intergers lo and hi.
uint32_t partition(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    uint32_t i = lo - 1;
    for (uint32_t j = lo; j < hi; j++) {
        if (cmp(stats, A[j - 1], A[hi - 1]) < 0) {
            i += 1;
            swap(stats, &A[i - 1], &A[j - 1]);
        }
    }
    swap(stats, &A[i], &A[hi - 1]);
    return i + 1;
}

// Recursive function used for quick sort.
// Code derived from asgn3.
// Takes in a struct stats, an array A, and 2 unsigned 32-bit intergers lo and hi.
void quick_sorter(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    uint32_t p;
    if (lo < hi) {
        p = partition(stats, A, lo, hi);
        quick_sorter(stats, A, lo, p - 1);
        quick_sorter(stats, A, p + 1, hi);
    }
}

// Sorts a given array via sorting algorithm called quick sort.
// Code derived from asgn3.
// Takes in a struct stats, an array A, and the length of the array n.
void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    quick_sorter(stats, A, 1, n);
}
