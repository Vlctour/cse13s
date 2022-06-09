#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <math.h>
#include "text.h"
#include "parser.h"
#include "ht.h"
#include "bv.h"
#include "bf.h"

#define WORD    "[a-zA-Z]+((-?[a-zA-Z]+)*('?[a-zA-Z]+)*)*"
#define HT_SIZE (1 << 19)
#define BF_SIZE (1 << 21)

// Got from Elmer
#define LWRCASE(c) (c - 'A' + 'a')
#define ISUP(c)    (c >= 'A' && c <= 'Z')
#define LWRFY(c)   (ISUP(c) ? LWRCASE(c) : c)

uint32_t noiselimit;

// Returns the absolute value.
double abso(double x) {
    double temp = x;
    return (temp < 0.0) ? -temp : temp;
}

// Create a struct called Text.
struct Text {
    HashTable *ht;
    BloomFilter *bf;
    uint32_t word_count;
};

// Creates a text on the heap.
// Returns a pointer to the text.
Text *text_create(FILE *infile, Text *noise) {
    Text *tx = (Text *) malloc(sizeof(Text));
    if (tx) {
        tx->word_count = 0;
        regex_t re;
        // If either the regex, ht, or bf failed, discontinue.
        if (regcomp(&re, WORD, REG_EXTENDED)) {
            fprintf(stderr, "Failed to compile regex\n");
            regfree(&re);
            free(tx);
            tx = NULL;
            return tx;
        }

        tx->ht = ht_create(HT_SIZE);
        if (tx->ht == NULL) {
            regfree(&re);
            free(tx);
            tx = NULL;
            return tx;
        }

        tx->bf = bf_create(BF_SIZE);
        if (tx->bf == NULL) {
            regfree(&re);
            ht_delete(&tx->ht);
            free(tx);
            tx = NULL;
            return tx;
        }

        // Check to see if we are making a noise text and also check noise limit.
        if (noise == NULL) {
            char *word = NULL;
            while (((word = next_word(infile, &re)) != NULL) && tx->word_count < noiselimit) {
                for (uint64_t i = 0; i < sizeof(word); i++) {
                    word[i] = LWRFY(word[i]);
                }
                ht_insert(tx->ht, word);
                bf_insert(tx->bf, word);
                tx->word_count++;
            }
            regfree(&re);
            noiselimit = tx->word_count;
            return tx;
        } else {
            char *word = NULL;
            while ((word = next_word(infile, &re)) != NULL) {
                for (uint64_t i = 0; i < sizeof(word); i++) {
                    word[i] = LWRFY(word[i]);
                }
                if (!(bf_probe(noise->bf, word))) {
                    ht_insert(tx->ht, word);
                    bf_insert(tx->bf, word);
                    tx->word_count++;
                }
            }
            regfree(&re);
            return tx;
        }
    }

    return tx;
}

// Compute the respective distance of one text to another, depending on the metric.
// Returns the distance between the two texts.
double text_dist(Text *text1, Text *text2, Metric metric) {
    if (text1 && text2) {
        HashTableIterator *hti1 = hti_create(text1->ht);
        HashTableIterator *hti2 = hti_create(text2->ht);
        double vec1, vec2, temp, total = 0.0;
        Node *n;

        // Computes words both in text1 and text2, and also words only in text1.
        while ((n = ht_iter(hti1)) != NULL) {
            if (text_contains(text2, n->word)) {
                vec1 = text_frequency(text1, n->word);
                vec2 = text_frequency(text2, n->word);
                if (metric == MANHATTAN) {
                    temp = vec1 - vec2;
                    total += abso(temp);
                } else if (metric == EUCLIDEAN) {
                    temp = vec1 - vec2;
                    abso(temp);
                    total += temp * temp;
                } else if (metric == COSINE) {
                    temp = vec1 * vec2;
                    total += temp;
                }

            } else {
                vec1 = text_frequency(text1, n->word);
                if (metric == MANHATTAN) {
                    temp = abso(vec1);
                    total += temp;
                } else if (metric == EUCLIDEAN) {
                    temp = abso(vec1);
                    total += temp * temp;
                } else if (metric == COSINE) {
                    temp = vec1 * 0;
                    total += temp;
                }
            }
        }
        hti_delete(&hti1);

        // Computes words only pertaining to text2.
        while ((n = ht_iter(hti2)) != NULL) {
            if (!(text_contains(text1, n->word))) {
                vec2 = text_frequency(text2, n->word);
                if (metric == MANHATTAN) {
                    temp = vec2;
                    total += abso(temp);
                } else if (metric == EUCLIDEAN) {
                    temp = vec2;
                    abso(temp);
                    total += temp * temp;
                } else if (metric == COSINE) {
                    temp = vec2 * 0;
                    total += temp;
                }
            }
        }
        hti_delete(&hti2);

        // Final computations.
        if (metric == EUCLIDEAN) {
            total = sqrt(total);
        } else if (metric == COSINE) {
            total = 1 - total;
        }
        return total;
    }

    return 0;
}

// Computes and returns the normalized value of a word.
double text_frequency(Text *text, char *word) {
    if (text && text_contains(text, word)) {
        return (double) ht_lookup(text->ht, word)->count / text->word_count;
    }

    return (double) 0;
}

// Checks to see if a word is in a text.
// This should be definitive.
// Returns true if its in the text, false otherwise.
bool text_contains(Text *text, char *word) {
    if (text) {
        if (bf_probe(text->bf, word)) {
            return ht_lookup(text->ht, word) != NULL ? true : false;
        }
    }
    return false;
}

// Deletes a text off the heap.
void text_delete(Text **text) {
    ht_delete(&(*text)->ht);
    bf_delete(&(*text)->bf);
    free(*text);
    *text = NULL;
    return;
}

// Prints out stuff pertaining to text_print.
void text_print(Text *text) {
    ht_print(text->ht);
    bf_print(text->bf);
    return;
}
