#include "heap.h"

// Find the max child of the tree/array.
// Code derived from asgn3.pdf.
// Takes in a struct stats, an array A, and 2 unsigned 32-bit intergers first and last.
// Returns an unsigned 32-bit interger.
uint32_t max_child(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if (right <= last && (cmp(stats, A[right - 1], A[left - 1]) > 0)) {
        return right;
    }
    return left;
}

// Fix the heap tree/array.
// Code derived from asgn3.pdf.
// Takes in a struct stats, an array A, and 2 unsigned 32-bit intergers first and last.
void fix_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    uint32_t found = 0;
    uint32_t mother = first;
    uint32_t great = max_child(stats, A, mother, last);
    while ((mother <= (last / 2)) && (found == 0)) {
        if (cmp(stats, A[mother - 1], A[great - 1]) < 0) {
            swap(stats, &A[mother - 1], &A[great - 1]);
            mother = great;
            great = max_child(stats, A, mother, last);
        } else {
            found = 1;
        }
    }
}

// Builds the heap into a tree format.
// Code derived from asgn3.pdf.
// Takes in a struct stats, an array A, and 2 unsigned 32-bit intergers first and last.
void build_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    for (uint32_t father = last / 2; father != first - 1; father -= 1) {
        fix_heap(stats, A, father, last);
    }
}

// Sorts an array using the Heap sort algorithm.
// Code derived from asgn3.pdf.
// Takes in a struct stats, an array A, and the length of the array n.
void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t first = 1, last = n;
    build_heap(stats, A, first, last);
    for (uint32_t leaf = last; leaf > first; leaf -= 1) {
        swap(stats, &A[first - 1], &A[leaf - 1]);
        fix_heap(stats, A, first, leaf - 1);
    }
}
