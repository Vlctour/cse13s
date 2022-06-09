#include "batcher.h"
#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Enumerate structure that contains the sorting algorithms.
typedef enum { insertion_alg = 0, heap_alg = 1, quick_alg = 2, batcher_alg = 3 } sorts;

// Prints out usage page if specified.
// Copied usage page information from asgn3 repo.
void usage(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "    A collection of comparison-based sorting algorithms.\n"
                    "\n"
                    "USAGE\n"
                    "    ./sorting [-Haibhqn:p:r:] [-n length] [-p elements] [-r seed]\n"
                    "    Example: ./sorting -a -n 100 -p 50 -r 10\n"
                    "\n"
                    "OPTIONS\n"
                    "    -H            Display program help and usage.\n"
                    "    -a            Enable all sorts.\n"
                    "    -i            Enable Insertion Sort.\n"
                    "    -b            Enable Batcher Sort.\n"
                    "    -h            Enable Heap Sort.\n"
                    "    -q            Enable Quick Sort.\n"
                    "    -n length     Specify number of array elements (default: 100).\n"
                    "    -p elements   Specify number of elements to print (default: 100).\n"
                    "    -r seed       Specify random seed (default: 13371453).\n");
}

// Psuedo Random number generator.
// Generates / Re-generates the psuedo random sequence.
// Stores the random values back into the location of the array.
void random_reset(uint32_t *A, uint32_t n, uint32_t seed) {
    uint32_t mask = 0x3FFFFFFF;
    srandom(seed);
    for (uint32_t i = 0; i < n; i++) {
        A[i] = random() & mask;
    }
}

// Takes in user input and runs specific sorting algorithms on a pseudo random array.
// Returns 0.
int main(int argc, char **argv) {
    int32_t opt = 0;
    Set s = empty_set();
    uint32_t seed = 13371453, n = 100, p = 100, p_counter = 0;
    char *names[] = { "Insertion Sort", "Heap Sort", "Quick Sort", "Batcher Sort" };
    bool empty = true;
    while ((opt = getopt(argc, argv, "-Haibhqn:p:r:")) != -1) {
        switch (opt) {
        case 'H': usage(); break;
        case 'a':
            s = complement_set(empty_set());
            empty = false;
            break;
        case 'i':
            s = insert_set(insertion_alg, s);
            empty = false;
            break;
        case 'b':
            s = insert_set(batcher_alg, s);
            empty = false;
            break;
        case 'h':
            s = insert_set(heap_alg, s);
            empty = false;
            break;
        case 'q':
            s = insert_set(quick_alg, s);
            empty = false;
            break;
        case 'n': n = strtod(optarg, NULL); break;
        case 'p': p = strtod(optarg, NULL); break;
        case 'r': seed = strtod(optarg, NULL); break;
        default: usage(); return EXIT_FAILURE;
        }
    }

    if (empty) {
        usage();
        return EXIT_FAILURE;
    }

    uint32_t *poi = (uint32_t *) malloc(n * sizeof(uint32_t));
    void (*fun[])(Stats * stats, uint32_t * A, uint32_t n)
        = { insertion_sort, heap_sort, quick_sort, batcher_sort };
    for (sorts picker = insertion_alg; picker <= batcher_alg; picker += 1) {
        if (member_set(picker, s)) {
            random_reset(poi, n, seed);
            Stats algo = { 0, 0 };
            fun[picker](&algo, poi, n);
            printf("%s, %d elements, %lu moves, %lu compares\n", names[picker], n, algo.moves,
                algo.compares);
            for (uint32_t i = 0; i < n; i++) {
                if (p_counter == p) {
                    break;
                }
                if (i % 5 == 0 && i != 0) {
                    printf("\n");
                }
                printf("%13" PRIu32, poi[i]);
                p_counter += 1;
            }
            printf("\n");
            p_counter = 0;
        }
    }
    free(poi);
    return 0;
}
