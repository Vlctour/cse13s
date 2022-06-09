#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ht.h"
#include "salts.h"
#include "speck.h"

// Make a struct for a hash table.
struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **nodes;
};

// Make a struct for a hash table iterator.
struct HashTableIterator {
    HashTable *table;
    uint32_t slot;
};

// Create a hash table on the heap.
// Return the pointer to the hash table.
HashTable *ht_create(uint32_t size) {
    HashTable *h = (HashTable *) malloc(sizeof(HashTable));
    if (h != NULL) {
        h->nodes = (Node **) calloc(size, sizeof(Node *));
        if (h->nodes != NULL) {
            h->salt[0] = SALT_HASHTABLE_LO;
            h->salt[1] = SALT_HASHTABLE_HI;
            h->size = size;
            return h;
        }
        free(h);
        h = NULL;
    }

    return h;
}

// Prints out the hash table.
// Used for debugging purposes
void ht_print(HashTable *ht) {
    HashTableIterator *hti = hti_create(ht);
    Node *n;
    while ((n = ht_iter(hti)) != NULL) {
        printf("Iter number: %u   ", hti->slot);
        node_print(n);
    }
    hti_delete(&hti);
    return;
}

// Deletes the hash table on the heap.
// Deletes the remaining nodes as well that are inside the hash table.
// Might need to iterate over the array of nodes to delete them.
void ht_delete(HashTable **ht) {
    if (*ht) {
        HashTableIterator *hti = hti_create(*ht);
        Node *temp;
        while ((temp = ht_iter(hti)) != NULL) {
            node_delete(&temp);
        }
        hti_delete(&hti);

        free((*ht)->nodes);
        (*ht)->nodes = NULL;
        free(*ht);
        *ht = NULL;
    }

    return;
}

// Get the size of the hash table.
// Unsure whether or not this means to get the total size or current size.
// Returns the size of the hash table.
uint32_t ht_size(HashTable *ht) {
    if (ht) {
        return ht->size;
    }

    return 0;
}

// See if a word is contained in the hash table.
// Influenced by lecture on Hash Tables.
// Points to the node that we are trying to search for.
Node *ht_lookup(HashTable *ht, char *word) {
    uint32_t count = 0;
    uint32_t pointer = hash(ht->salt, word) % ht->size;

    while (count < ht->size) {
        Node *item = ht->nodes[pointer];

        if (item && strcmp(item->word, word) == 0) {
            return item;
        }

        pointer = (pointer + 1) % ht->size;
        count++;
    }

    return NULL;
}

// Insert a word into the hash table.
// Influenced by the lecture on Hash Tables.
// Returns a pointer to the node inserted.
Node *ht_insert(HashTable *ht, char *word) {
    uint32_t count = 0;
    uint32_t pointer = hash(ht->salt, word) % ht->size;
    while (count < ht->size) {
        if (ht->nodes[pointer] == NULL) {
            ht->nodes[pointer] = node_create(word);
            ht->nodes[pointer]->count += 1;
            return ht->nodes[pointer];
        } else if (strcmp(ht->nodes[pointer]->word, word) == 0) {
            ht->nodes[pointer]->count += 1;
            return ht->nodes[pointer];
        }

        pointer = (pointer + 1) % ht->size;
        count++;
    }

    return NULL;
}

// Create an interator over the hash table.
// Return the pointer to the hash table iterator.
HashTableIterator *hti_create(HashTable *ht) {
    HashTableIterator *hti = (HashTableIterator *) malloc(sizeof(HashTableIterator));
    if (hti) {
        hti->table = ht;
        if (hti->table) {
            hti->slot = 0;
            return hti;
        }
        free(hti);
        hti = NULL;
    }

    return hti;
}

// Deletes the hash table iterator on the heap.
void hti_delete(HashTableIterator **hti) {
    if (*hti) {
        free(*hti);
        *hti = NULL;
    }

    return;
}

// Iterates over a given hash table.
// Returns a valid node.
Node *ht_iter(HashTableIterator *hti) {
    uint32_t index;
    while (hti->slot < hti->table->size) {
        if (hti->table->nodes[hti->slot] != NULL) {
            index = hti->slot;
            hti->slot++;
            //node_print(hti->table->nodes[index]);
            return hti->table->nodes[index];
        } else {
            hti->slot++;
        }
    }
    return NULL;
}
