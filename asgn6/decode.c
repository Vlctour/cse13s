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

// Prints a helpful usage page for decode.c.
void usage(char *exe) {
    fprintf(stderr,
        "Synopsis\n"
        "    A Huffman decoder.\n"
        "    Decompresses a file using the Huffman coding algorithm.\n"
        "\n"
        "Usage\n"
        "    %s [-h] [-i infile] [-o outfile]\n"
        "\n"
        "Options\n"
        "    -h            Display program help and usage page.\n"
        "    -v            Print compression statistics.\n"
        "    -i infile     Input file to decompress. (default: stdin)\n"
        "    -o outfile    Output of decompressed data. (default: stdout)\n",
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
            = open("huffman4_tmp5_file_3_that2_is1_unique0.dat", O_CREAT | O_TRUNC | O_RDWR, 0600);
        unlink("huffman4_tmp5_file_3_that2_is1_unique0.dat");

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

// This program decodes a file specified by the user.
// Code follows assign6 specifications.
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

    // Make an array of uint8 that is of the size of a header.
    // Read in a Header size of data.
    // Cast the bytes as a Header.
    // Check if header magic number matches ours.
    // Change perms for outfile to match.
    uint8_t heade[sizeof(Header)];
    read_bytes(infile, heade, sizeof(Header));
    Header h = *((Header *) &heade);
    if (h.magic != MAGIC) {
        fprintf(stderr, "Magic number doesn't match\n");
        return EXIT_FAILURE;
    }
    fchmod(outfile, h.permissions);

    // Make a buffer for that is the size of the tree.
    // Make another buffer of the size of a BLOCK.
    uint8_t buffer_tree[h.tree_size];
    uint8_t file_buffer[BLOCK];
    uint8_t bit_read;
    uint64_t file_buffer_pointer = 0, file_total = 0;

    // Read in the dumped tree.
    read_bytes(infile, buffer_tree, h.tree_size);

    // Reconstruct the tree.
    Node *root = rebuild_tree(h.tree_size, buffer_tree);
    Node *temp_node = root;

    // Starting at the beginning of the file, write the corresponding character for each code
    // to the outfile.
    while ((file_total < h.file_size)) {
        read_bit(infile, &bit_read);
        if (bit_read) {
            temp_node = temp_node->right;
        } else {
            temp_node = temp_node->left;
        }

        if (!(temp_node->left) && !(temp_node->right)) {
            file_buffer[file_buffer_pointer] = temp_node->symbol;
            file_buffer_pointer++;
            file_total++;
            temp_node = root;

            if (file_buffer_pointer == BLOCK) {
                write_bytes(outfile, file_buffer, BLOCK);
                file_buffer_pointer = 0;
            }
        }
    }
    write_bytes(outfile, file_buffer, file_buffer_pointer);

    // Verbose/Statistics.
    if (v) {
        double space = 100 * (1 - ((float) bytes_read / (float) bytes_written));
        printf("Compressed file size: %lu bytes\n", bytes_read);
        printf("Decompressed file size: %lu bytes\n", bytes_written);
        printf("Space saving: %.2f"
               "%s\n",
            space, "%");
    }

    close(infile);
    close(outfile);
    delete_tree(&root);
    return 0;
}
