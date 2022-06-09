#include <stdio.h>
#include <stdlib.h>
#include "universe.h"
#include <inttypes.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int opt = 0;
    //char hi;
    //FILE *file;
    int row_len, col_len;
    while ((opt = getopt(argc, argv, "i:")) != -1) {
        switch (opt) {
        case 'i':
            fscanf(stdin, "%d %d", &row_len, &col_len);
            printf("%d %d\n", row_len, col_len);
            //sscanf(optarg, "%s", &hi);
            //printf("%s\n", optarg);
        }
    }
    fscanf(stdin, "%d %d", &row_len, &col_len);
    printf("%d %d\n", row_len, col_len);
    return 0;
    //uint32_t coun = 0, c = 4293967295, tot;
    //Universe *x = uv_create(10, 10, false);

    //FILE *files = fopen("corner.txt", "r");
    //uv_populate(x, files);
    //fclose(files);

    //FILE *out = fopen("testinggg.txt", "w");
    //uv_print(x, out);
    //fclose(out);
    //coun = uv_census(x, 0, 0);
    //tot = coun + c;
    //printf("add:%d coun:%d c:%d\n", tot, coun, c);

    //uv_delete(x);
}
