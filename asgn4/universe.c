#include "universe.h"
#include <stdlib.h>

// Create a struct called Universe.
struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

// Create the actual universe on the heap.
// Return the pointer to the universe.
Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *uni = (Universe *) malloc(sizeof(Universe));
    uni->rows = rows;
    uni->cols = cols;
    uni->grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t r = 0; r < rows; r += 1) {
        uni->grid[r] = (bool *) calloc(cols, sizeof(bool));
    }
    uni->toroidal = toroidal;
    return uni;
}

// Free the heap of the universe that we created.
void uv_delete(Universe *u) {
    for (uint32_t r = 0; r < uv_rows(u); r += 1) {
        free(u->grid[r]);
        u->grid[r] = NULL;
    }
    free(u->grid);
    u->grid = NULL;
    free(u);
    u = NULL;
}

// Given a universe, returns the number of rows.
uint32_t uv_rows(Universe *u) {
    return u->rows;
}

// Given a universe, return the number of columns.
uint32_t uv_cols(Universe *u) {
    return u->cols;
}

// Given a universe, make the cell alive, given the row and column.
void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= 0 && c >= 0 && r < uv_rows(u) && c < uv_cols(u)) {
        u->grid[r][c] = true;
    }
}

// Given a universe, make the cell dead, given the row and column.
void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= 0 && c >= 0 && r < uv_rows(u) && c < uv_cols(u)) {
        u->grid[r][c] = false;
    }
}

// Given a universe, check the state of a cell at the given row and column.
bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= 0 && c >= 0 && r < uv_rows(u) && c < uv_cols(u)) {
        return u->grid[r][c];
    } else {
        return false;
    }
}

// Given a universe, read the specified file and make the cells alive where ever specified.
// If the specified row and column is outside of the 2d array, return false.
// Return true if all row and columns were populated.
bool uv_populate(Universe *u, FILE *infile) {
    uint32_t x, y;
    while (fscanf(infile, "%d %d", &x, &y) == 2) {
        uv_live_cell(u, x, y);
        if ((x < 0) || (x > uv_rows(u)) || (y < 0) || (y > uv_cols(u))) {
            return false;
        }
    }
    if (fscanf(infile, "%d %d", &x, &y) != EOF) {
        return false;
    }
    return true;
}

// Given a universe, locate the specified cell from the given row and column.
// Find the number of cells alive that are around the specified cell.
// Return the number of cells alive.
uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t nei[][8]
        = { { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 }, { -1, 1 }, { 1, 1 }, { 1, -1 }, { -1, -1 } };
    uint32_t count = 0;
    if (u->toroidal) {
        for (uint32_t i = 0; i < 8; i++) {
            count = uv_get_cell(u, (r + uv_rows(u) + nei[i][0]) % uv_rows(u),
                        (c + uv_cols(u) + nei[i][1]) % uv_cols(u))
                        ? count + 1
                        : count;
        }
    } else {
        for (uint32_t i = 0; i < 8; i++) {
            if ((nei[i][0] + r >= 0) && (nei[i][0] + r < uv_rows(u)) && (nei[i][1] + c >= 0)
                && (nei[i][1] + c < uv_cols(u))) {
                count = uv_get_cell(u, r + nei[i][0], c + nei[i][1]) ? count + 1 : count;
            }
        }
    }
    return count;
}

// Given a universe, print out the universe.
// Output file should be given.
void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t i = 0; i < uv_rows(u); i++) {
        for (uint32_t j = 0; j < uv_cols(u); j++) {
            if (u->grid[i][j]) {
                fprintf(outfile, "o");
            } else {
                fprintf(outfile, ".");
            }
        }
        fprintf(outfile, "\n");
    }
}
