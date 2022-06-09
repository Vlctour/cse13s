#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ncurses.h>
#include "universe.h"

#define DELAY 50000

// Prints a helpful usage page for life.c.
void usage(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "    Print out the game of life.\n"
                    "\n"
                    "USAGE\n"
                    "    ./life [-Htsn:i:o:] [-n generations] [-i input] [-o output]\n"
                    "\n"
                    "OPTIONS\n"
                    "    -H                Display program help and usage.\n"
                    "    -t                Enables game to be played on toroidal universe.\n"
                    "    -s                Silence ncurses. Disables display.\n"
                    "    -n generations    Specify number of generations. (Default: 100)\n"
                    "    -i input          Specify input file to read. (Default: stdin)\n"
                    "    -o output         Specify output file to print. (Default: stdout)\n");
}

// Function to swap universes.
void swap(Universe **a, Universe **b, bool t) {
    Universe *temp = uv_create(uv_rows(*a), uv_cols(*a), t);
    uv_delete(temp);
    temp = *a;
    *a = *b;
    *b = temp;
}

// Takes in user input and prints out the game of life.
// Returns 0.
int main(int argc, char **argv) {
    int32_t opt = 0;
    char *input, *output;
    FILE *in_file, *out_file;
    uint32_t n = 100, row_len, col_len;
    bool t = false, s = false, f_specifier = false, o_specifier = false, no_crash = true;
    while ((opt = getopt(argc, argv, "Htsn:i:o:")) != -1) {
        switch (opt) {
        case 't': t = true; break;
        case 's': s = true; break;
        case 'n': n = strtod(optarg, NULL); break;
        case 'i':
            f_specifier = true;
            input = optarg;
            break;
        case 'o':
            o_specifier = true;
            output = optarg;
            break;
        case 'H': usage(); break;
        default: usage(); return EXIT_FAILURE;
        }
    }

    // Opens the file and grabs the first two inputs to create the universes.
    // By default, we should be reading from stdin, but if a file is specified,
    // we'll read from getopt.
    if (f_specifier) {
        in_file = fopen(input, "r");
        if (in_file == NULL) {
            printf("Error opening %s.\n", input);
            return EXIT_FAILURE;
        }
        no_crash = fscanf(in_file, "%d %d", &row_len, &col_len);
    } else {
        no_crash = fscanf(stdin, "%d %d", &row_len, &col_len);
    }

    if (no_crash == false) {
        printf("Malformed input.\n");
        return EXIT_FAILURE;
    }

    Universe *a = uv_create(row_len, col_len, t);
    Universe *b = uv_create(row_len, col_len, t);

    if (f_specifier) {
        no_crash = uv_populate(a, in_file);
        if (no_crash == false) {
            printf("Malformed input.\n");
            return EXIT_FAILURE;
        }
        fclose(in_file);
    } else {
        no_crash = uv_populate(a, stdin);
        if (no_crash == false) {
            printf("Malformed input.\n");
            return EXIT_FAILURE;
        }
    }

    // Prints out the universe using ncurses.
    initscr();
    for (uint32_t gen = 0; gen < n; gen++) {
        if (s == false) {
            clear();
            for (uint32_t r = 0; r < row_len; r++) {
                for (uint32_t c = 0; c < col_len; c++) {
                    mvprintw(r, c, uv_get_cell(a, r, c) ? "o" : " ");
                    refresh();
                }
            }
            usleep(DELAY);
        }
        for (uint32_t r = 0; r < row_len; r++) {
            for (uint32_t c = 0; c < col_len; c++) {
                if (uv_get_cell(a, r, c)) {
                    if ((uv_census(a, r, c) == 2) || (uv_census(a, r, c) == 3)) {
                        uv_live_cell(b, r, c);
                    } else {
                        uv_dead_cell(b, r, c);
                    }
                } else {
                    if (uv_census(a, r, c) == 3) {
                        uv_live_cell(b, r, c);
                    } else {
                        uv_dead_cell(b, r, c);
                    }
                }
            }
        }
        swap(&a, &b, t);
    }
    endwin();

    // Prints to a specified outputfile if specified.
    // By default, we will print out to stdout.
    if (o_specifier) {
        out_file = fopen(output, "w");
        uv_print(a, out_file);
        fclose(out_file);
    } else {
        uv_print(a, stdout);
    }

    uv_delete(a);
    uv_delete(b);
    return 0;
}
