#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "node.h"
#include "bv.h"
#include "ht.h"
#include "bf.h"
#include "text.h"
#include "pq.h"
#include "metric.h"

#define MAX_SIZE 2048

// Prints a helpful usage page on how to use the program.
void usage(char *exe) {
    fprintf(stderr,
        "Synopsis\n"
        "    Identifies the author of some anonymous text passed through stdin.\n"
        "    Basically computes k-nearest neighbors, where k = 1 by default.\n"
        "    The number of noise words that are removed is tunable.\n"
        "    Uses either Euclidean, Manhattan, or cosine distance as metric.\n"
        "\n"
        "Usage\n"
        "    %s [OPTIONS]\n"
        "\n"
        "Options\n"
        "    -d database     Database of authors and texts [default: lib.db]\n"
        "    -k matches      Set the number of top matches to display [default: 5]\n"
        "    -n noisefile    Set file of words to ignore [default: noise.txt]\n"
        "    -l limit        Set noise word limit [default: 100]\n"
        "    -e              Set distance metric as Euclidean distance [default]\n"
        "    -m              Set distance metric as Manhattan distance\n"
        "    -c              Set distance metric as Cosine distance\n"
        "    -v              Enable verbose printing of the program\n"
        "    -h              Display program help and usage\n",
        exe);
}

// The Author Identification.
// Identifies and returns the top k likely authors that are closely
// related to a given anonymous writer.
int main(int argc, char **argv) {
    int opt = 0;
    noiselimit = 100;
    double dist_num, final;
    bool dequeable = true;
    Metric select = EUCLIDEAN;
    char *d = "lib.db", *n = "noise.txt", *a;
    FILE *noise_f, *data_base, *author_file;
    uint32_t k = 5, data_base_num, actual_count = 0, counter = 1, pairs = 0;

    while ((opt = getopt(argc, argv, "d:n:k:l:emch")) != -1) {
        switch (opt) {
        case 'd': d = optarg; break;
        case 'n': n = optarg; break;
        case 'k': k = strtoul(optarg, NULL, 10); break;
        case 'l': noiselimit = strtoul(optarg, NULL, 10); break;
        case 'e': select = EUCLIDEAN; break;
        case 'm': select = MANHATTAN; break;
        case 'c': select = COSINE; break;
        case 'h': usage(argv[0]); return EXIT_FAILURE;
        default: usage(argv[0]); return EXIT_FAILURE;
        }
    }

    // Creates a noise file and read in anonymous file through stdin.
    noise_f = fopen(n, "r");
    if (noise_f == NULL) {
        fprintf(stderr, "ERROR OPENING %s.\n", n);
        return EXIT_FAILURE;
    }
    Text *noiseee = text_create(noise_f, NULL);
    Text *anon = text_create(stdin, noiseee);

    // Opens and collects number of authors to compare too.
    data_base = fopen(d, "r");
    if (data_base == NULL) {
        fprintf(stderr, "ERROR OPENING %s.\n", d);
        return EXIT_FAILURE;
    }

    if (fscanf(data_base, "%d\n", &data_base_num) != 1) {
        fprintf(stderr, "INVALID INPUT IN DATA BASE.\n");
        return EXIT_FAILURE;
    }

    PriorityQueue *q = pq_create(data_base_num + 1);

    // Read the rest of the file, compute the distances, and enqueue the authors into the
    // priority queue.
    char a_name[MAX_SIZE], a_loc[MAX_SIZE];
    do {
        fgets(a_name, sizeof(a_name), data_base);
        fgets(a_loc, sizeof(a_loc), data_base);
        a_name[strlen(a_name) - 1] = '\0';
        a_loc[strlen(a_loc) - 1] = '\0';
        author_file = fopen(a_loc, "r");
        if (author_file) {
            actual_count++;
            Text *t = text_create(author_file, noiseee);
            dist_num = text_dist(anon, t, select);
            enqueue(q, strdup(a_name), dist_num);
            fclose(author_file);
            text_delete(&t);
        } else {
            fprintf(stderr, "No such file, or directory.\n");
        }
        pairs++;

    } while (pairs < data_base_num);

    // Print out the top k likely authors.
    printf("Top %d, metric: %s, noise limit: %d\n", k, metric_names[select], noiselimit);
    while (counter <= k && dequeable) {
        dequeable = dequeue(q, &a, &final);
        if (dequeable) {
            printf("%d) %s [%0.15f]\n", counter, a, final);
            counter++;
        }
    }

    // Free everything used and close files.
    pq_delete(&q);
    text_delete(&noiseee);
    text_delete(&anon);
    fclose(noise_f);
    fclose(data_base);
    return 0;
}
