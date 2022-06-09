#include "mathlib.h"

#include <math.h>
#include <stdio.h>

// Compares my math library that I created to the shared math.h library and prints their numbers
// out along with the difference.
// I editted this testfile a lot to test different numbers.
// Returns 0.
int main(void) {
    int size = 4;
    double diff;
    double list[] = { 0, 1, -1, -2 };
    for (int i = 0; i < size; i++) {
        diff = sqrt(list[i]) - Sqrt(list[i]);
        printf("My number %0.14f Computer num %0.14f diff: %0.14f\n", Sqrt(list[i]), sqrt(list[i]),
            diff);
    }
    return 0;
}
