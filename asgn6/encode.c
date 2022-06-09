#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <inttypes.h>
#include "defines.h"
#include "node.h"
#include "io.h"
#include "huffman.h"
#include "header.h"

// Prints a helpful usage page for encode.c.
void usage(char *exe) {
    fprintf(stderr,
        "Synopsis\n"
        "    A Huffman encoder.\n"
        "    Compresses a file using the Huffman coding algorithm.\n"
        "\n"
        "Usage\n"
        "    %s [-h] [-i infile] [-o outfile]\n"
        "\n"
        "Options\n"
        "    -h            Display program help and usage page.\n"
        "    -v            Print compression statistics.\n"
        "    -i infile     Input file to compress. (default: stdin)\n"
        "    -o outfile    Output of compressed data. (default: stdout)\n",
        exe);
}

// Create a temporary file to be able to read stuff from stdin.
// Eugene helped me make this.
// Return the fd for the new temporary file.
int make_tempf(int infile) {
    int readed = 0;
    uint8_t buffer[] = { 'a' };
    if (lseek(infile, 0, SEEK_CUR) == -1) {
        int outfile
            = open("huffman0_tmp1_file_2_that3_is5_unique4.dat", O_CREAT | O_TRUNC | O_RDWR, 0600);
        unlink("huffman0_tmp1_file_2_that3_is5_unique4.dat");

        do {
            readed = read(infile, buffer, sizeof(buffer));
            write(outfile, buffer, readed);
        } while (readed > 0);

        lseek(outfile, 0, SEEK_SET);
        close(infile);
        infile = outfile;
    }

    return infile;
}

// This program encodes a file specified by the user.
// Code follows asgn6 specifications.
// Takes in user arguments and parses them through getopt.
//
// Returns 0.
int main(int argc, char **argv) {
    int opt;
    int infile, outfile = STDOUT_FILENO;
    bool v = false, in_speci = false;
    while ((opt = getopt(argc, argv, "hvi:o:")) != -1) {
        switch (opt) {
        case 'i':
            infile = open(optarg, O_RDONLY, 0600);
            in_speci = true;
            break;
        case 'o': outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0600); break;
        case 'v': v = true; break;
        case 'h': usage(argv[0]); return EXIT_FAILURE;
        default: usage(argv[0]); return EXIT_FAILURE;
        }
    }

    // Check if infile was specified, if not use stdin temp file.
    // Also check if files were openable.
    if (!in_speci) {
        infile = make_tempf(STDIN_FILENO);
    }
    if (infile == -1) {
        fprintf(stderr, "ERROR OPENING INFILE.\n");
        return EXIT_FAILURE;
    }
    if (outfile == -1) {
        fprintf(stderr, "ERROR OPENING OUTFILE.\n");
        return EXIT_FAILURE;
    }

    // Initialize histogram, set all values to 0.
    uint64_t histo[ALPHABET];
    for (int i = 0; i < ALPHABET; i++) {
        histo[i] = 0;
    }

    // Make a buffer and count.
    uint8_t buffer[BLOCK], buffer_2[BLOCK];
    int readed;

    // While there is still stuff to read, increment the corresponding char in histogram by 1.
    while ((readed = read_bytes(infile, buffer, BLOCK)) > 0) {
        for (int i = 0; i < readed; i++) {
            histo[buffer[i]] += 1;
        }
    }

    // First initial read shouldn't count towards bytes_read.
    bytes_read = 0;

    // Increment element 0 and 255 to guarantee the minimum number of elements.
    histo[0] += 1;
    histo[255] += 1;

    // Calculate the unique number of elements in a histogram.
    uint16_t unique_sym = 0;
    for (int j = 0; j < ALPHABET; j++) {
        if (histo[j] > 0) {
            unique_sym += 1;
        }
    }

    // Build tree.
    Node *root = build_tree(histo);

    // Make code table.
    Code table[ALPHABET];
    build_codes(root, table);

    // Make header.
    // Change perms for outfile to match.
    // Cast header to an array of uint8s.
    struct stat stat_buff[sizeof(struct stat)];
    fstat(infile, stat_buff);
    Header h;
    h.magic = MAGIC;
    h.permissions = stat_buff->st_mode;
    h.tree_size = (3 * unique_sym) - 1;
    h.file_size = stat_buff->st_size;
    uint8_t *heade = ((uint8_t *) &h);
    fchmod(outfile, stat_buff->st_mode);

    // Write out the header.
    write_bytes(outfile, heade, sizeof(Header));

    // Dump tree
    dump_tree(outfile, root);

    // Start at beginning of infile and write the corresponding bits for each char in infile.
    lseek(infile, 0, SEEK_SET);
    while ((readed = read_bytes(infile, buffer_2, BLOCK)) > 0) {
        for (int k = 0; k < readed; k++) {
            write_code(outfile, &table[buffer_2[k]]);
        }
    }
    flush_codes(outfile);

    // Verbose/Statistics.
    if (v) {
        double space = 100 * (1 - ((float) bytes_written / (float) bytes_read));
        printf("Uncompressed file size: %lu bytes\n", bytes_read);
        printf("Compressed file size: %lu bytes\n", bytes_written);
        printf("Space saving: %.2f"
               "%s\n",
            space, "%");
    }

    close(infile);
    close(outfile);
    delete_tree(&root);
    return 0;
}
