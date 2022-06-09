#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include "node.h"
#include "pq.h"
#include "code.h"
#include "io.h"
#include "stack.h"

int main(void) {
    Code yo = code_init();
    //printf("%u\n", code_size(&yo));
    //printf("is it empty %d\n", code_empty(&yo));
    //printf("is it full %d\n", code_full(&yo));
    //printf("\n");
    /*
    printf("set 0th, 1st, 2nd, 7th, 251st bits\n");
    code_set_bit(&yo, 0);
    code_set_bit(&yo, 1);
    code_set_bit(&yo, 2);
    code_set_bit(&yo, 7);
    code_set_bit(&yo, 251);
    code_set_bit(&yo, 255);
    code_clr_bit(&yo, 251);
    code_print(&yo);
    */
    ///*
    printf("was grabbing bit successful %d\n", code_get_bit(&yo, 3));
    uint8_t wh = 0;
    code_push_bit(&yo, 1);
    code_push_bit(&yo, 0);
    code_pop_bit(&yo, &wh);
    printf("ah %hhu\n", wh);
    code_pop_bit(&yo, &wh);
    printf("huh %hhu\n", wh);
    code_print(&yo);
    code_push_bit(&yo, 0);
    code_push_bit(&yo, 1);
    code_push_bit(&yo, 1);
    code_push_bit(&yo, 0);
    code_push_bit(&yo, 0);
    code_push_bit(&yo, 0);
    code_push_bit(&yo, 1);
    code_push_bit(&yo, 1);
    code_push_bit(&yo, 1);
    code_print(&yo);
    //*/

    return 0;
}
