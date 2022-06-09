#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include "node.h"
#include "ht.h"
#include "bv.h"
#include "bf.h"
#include "parser.h"
#include "text.h"
#include "metric.h"
#include "pq.h"

// does it break?
#define W "[a-zA-Z]+((-?[a-zA-Z]+)*('?[a-zA-Z]+)*)*"

void node_test(void) {
    char *word = { "hello" };
    Node *n = node_create(word);
    node_print(n);
    node_delete(&n);
    return;
}

void hash_test(void) {
    char *word = { "yo" }, *word2 = { "world" }, *word3 = { "hell" }, *word4 = { "balls" },
         *word5 = { "g" };
    char *word6 = { "c" }, *word7 = { "cow" }, *word8 = { "eye" }, *word9 = { "bob" },
         *word10 = { "ape" };
    HashTable *h = ht_create(10);
    ht_insert(h, word);
    ht_insert(h, word2);
    ht_insert(h, word3);
    ht_insert(h, word4);
    ht_insert(h, word5);
    ht_insert(h, word6);
    ht_insert(h, word7);
    ht_insert(h, word8);
    ht_insert(h, word9);
    ht_insert(h, word9);
    ht_insert(h, word10);
    ht_print(h);

    ht_delete(&h);
    return;
}

void bv_test(void) {
    BitVector *bv = bv_create(4);
    printf("length %d\n", bv_length(bv));
    for (uint32_t i = 0; i < 5; i++) {
        printf("index: %d successful: %d\n", i, bv_set_bit(bv, i));
    }
    bv_print(bv);
    bv_delete(&bv);
}

void bf_test(void) {
    char *word = { "hey" };
    BloomFilter *bf = bf_create(100);
    bf_print(bf);
    bf_insert(bf, word);
    bf_print(bf);
    printf("is \"hey\" in the bloom filter? %d\n", bf_probe(bf, word));
    bf_delete(&bf);
}

int reg_test(void) {
    regex_t re;
    if (regcomp(&re, W, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile.\n");
        return 1;
    }

    char *word = NULL;
    while ((word = next_word(stdin, &re)) != NULL) {
        printf("Word: %s\n", word);
    }

    return 0;
}

void text_test(void) {
    noiselimit = 5;
    FILE *infile = fopen("me.txt", "r");
    FILE *infile2 = fopen("me2.txt", "r");
    FILE *noisefile = fopen("limit.txt", "r");
    Text *noise = text_create(noisefile, NULL);
    //text_print(noise);
    Text *tx = text_create(infile, noise);
    Text *tx2 = text_create(infile2, noise);

    //text_print(noise);
    text_print(tx);
    text_print(tx2);

    double x = text_dist(tx, tx2, MANHATTAN);
    printf("dist %0.3f\n", x);

    text_delete(&tx2);
    text_delete(&tx);
    text_delete(&noise);
    fclose(noisefile);
    fclose(infile2);
    fclose(infile);
}

void metric_test(void) {
    for (Metric m = 0; m < 3; m++) {
        printf("%s\n", metric_names[m]);
    }
}

void pq_test(void) {
    char *temp;
    double te;
    double num = 0.2;
    PriorityQueue *q = pq_create(10);
    FILE *infile = fopen("names.txt", "r");
    char na[1000];

    for (uint32_t i = 0; i < 4; i++) {
        fgets(na, sizeof(na), infile);

        na[strlen(na) - 1] = '\0';
        char *y = strdup(na);
        enqueue(q, y, num);
        //pq_print(q);
        //free(y);
        num++;

        fgets(na, sizeof(na), infile);
        na[strlen(na) - 1] = '\0';
        pq_print(q);
    }
    fclose(infile);
    /*
    enqueue(q, "eugene", 0.10);
    enqueue(q, "victor", 0.50);
    enqueue(q, "ben", 0.20);
    enqueue(q, "santosh", 0.15);
    enqueue(q, "gecko", 0.20);
    enqueue(q, "jason", 0.70);
    */
    ///*
    //pq_print(q);
    for (uint32_t i = 0; i < 4; i++) {
        dequeue(q, &temp, &te);
        printf("popped %s, %f\n", temp, te);
        free(temp);
        //pq_print(q);
    }
    //*/
    pq_delete(&q);
}

int main(void) {
    //node_test();
    //hash_test();
    //bv_test();
    //bf_test();
    reg_test();
    //text_test();
    //metric_test();
    //printf("%d\n", 1 << 19);
    //pq_test();
    return 0;
}
