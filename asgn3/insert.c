#include "insert.h"

// Given a list, sorts the list via Insertion Sort.
// Code derived from asgn3.pdf.
// Takes in a struct stats, an array A, and the length of the array n.
void insertion_sort(Stats *stats, uint32_t *A, uint32_t n) {
    for (uint32_t i = 1; i < n; i++) {
        uint32_t j = i, temp = move(stats, A[i]);
        while (j > 0 && (cmp(stats, temp, A[j - 1]) < 0)) {
            A[j] = move(stats, A[j - 1]);
            j -= 1;
        }
        A[j] = move(stats, temp);
    }
}
