#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

uint32_t b1(uint32_t n) {
    uint32_t count = 0;
    while (n > 0) {
        count += 1;
        n >>= 1;
    }
    return count;
}

int main(void) {
    uint32_t test = 1234567891;
    uint32_t mask = 0x3FFFFFFF;
    uint32_t new;

    uint32_t n = 2;
    uint32_t arr[n];
    uint32_t *p = arr;
    uint32_t seed = 13371453;
    srandom(seed);
    for (uint32_t i = 0; i < n; i++) {
        p[i] = random();
    }

    for (uint32_t i = 0; i < n; i++) {
        printf("val: %d\n", p[i]);
    }

    new = test &mask;
    printf("%d\n", new);

    uint32_t t = 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;

    printf("%d\n", b1(t));
}
