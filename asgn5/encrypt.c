#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <gmp.h>
#include "randstate.h"
#include "numtheory.h"
#include "rsa.h"

// Prints out useful usage page on how to use the program.
void usage(char *exe) {
    fprintf(stderr,
        "Synopsis\n"
        "    Encrypts data using RSA encryption.\n"
        "    Encrypted data is decrypted by the decrypt program.\n"
        "\n"
        "Usage\n"
        "    %s [OPTIONS]\n"
        "\n"
        "Options\n"
        "    -h            Display program help and usage page.\n"
        "    -v            Display verbose program output.\n"
        "    -i infile    Input file of data to encrypt (default: stdin).\n"
        "    -o outfile    Output file for encrypted data (default: stdout).\n"
        "    -n pbfile     Public key file (default: rsa.pub).\n",
        exe);
}

// This program encrypts a file specified by the user.
// Code is influenced by the asgn5 specifications.
// Takes in user arguments and parses them through getopt.
//
// Returns 0.
int main(int argc, char **argv) {
    int64_t opt = 0;
    bool v = false;
    FILE *input = stdin, *output = stdout, *pbfile;
    char *pb_file_name = "rsa.pub", username[256];

    while ((opt = getopt(argc, argv, "i:o:n:vh")) != -1) {
        switch (opt) {
        case 'i':
            input = fopen(optarg, "r");
            if (input == NULL) {
                fprintf(stderr, "ERROR OPENING %s.\n", optarg);
                return EXIT_FAILURE;
            }
            break;
        case 'o':
            output = fopen(optarg, "w");
            if (output == NULL) {
                fprintf(stderr, "ERROR OPENING %s.\n", optarg);
                return EXIT_FAILURE;
            }
            break;
        case 'n': pb_file_name = optarg; break;
        case 'v': v = true; break;
        case 'h': usage(argv[0]); return EXIT_FAILURE;
        default: usage(argv[0]); return EXIT_FAILURE;
        }
    }

    mpz_t u, n, e, s;
    mpz_inits(u, n, e, s, NULL);

    pbfile = fopen(pb_file_name, "r");

    if (pbfile == NULL) {
        printf("ERROR OPENING %s.\n", pb_file_name);
        mpz_clears(n, e, s, u, NULL);
        return EXIT_FAILURE;
    }

    rsa_read_pub(n, e, s, username, pbfile);

    fclose(pbfile);

    if (v) {
        gmp_printf("user = %s\n"
                   "s (%d bits) = %Zd\n"
                   "n (%d bits) = %Zd\n"
                   "e (%d bits) = %Zd\n",
            username, mpz_sizeinbase(s, 2), s, mpz_sizeinbase(n, 2), n, mpz_sizeinbase(e, 2), e);
    }

    mpz_set_str(u, username, 62);

    if (!rsa_verify(u, s, e, n)) {
        printf("Unable to verify signature.\n");
        mpz_clears(n, e, s, u, NULL);
        return EXIT_FAILURE;
    }

    rsa_encrypt_file(input, output, n, e);

    fclose(input);
    fclose(output);
    mpz_clears(n, e, s, u, NULL);
    return 0;
}
