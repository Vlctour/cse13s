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
        "    Decrypts data using RSA encryption.\n"
        "    Encrypted data is encrypted by the encrypt program.\n"
        "\n"
        "Usage\n"
        "    %s [OPTIONS]\n"
        "\n"
        "Options\n"
        "    -h            Display program help and usage page.\n"
        "    -v            Display verbose program output.\n"
        "    -i innfile    Input file of data to decrypt (default: stdin).\n"
        "    -o outfile    Output file for decrypt data (default: stdout).\n"
        "    -n pvfile     Private key file (default: rsa.priv).\n",
        exe);
}

// This program decrypts a file specified by the user.
// Code is influenced by the asgn5 specifications.
// Takes in user arguments and parses them through getopt.
//
// Returns 0.
int main(int argc, char **argv) {
    int64_t opt = 0;
    bool v = false;
    FILE *input = stdin, *output = stdout, *pvfile;
    char *pv_file_name = "rsa.priv";

    while ((opt = getopt(argc, argv, "i:o:n:vh")) != -1) {
        switch (opt) {
        case 'i':
            input = fopen(optarg, "r");
            if (input == NULL) {
                printf("ERROR OPENING %s.\n", optarg);
                return EXIT_FAILURE;
            }
            break;
        case 'o':
            output = fopen(optarg, "w");
            if (output == NULL) {
                printf("ERROR OPENING %s.\n", optarg);
                return EXIT_FAILURE;
            }
            break;
        case 'n': pv_file_name = optarg; break;
        case 'v': v = true; break;
        case 'h': usage(argv[0]); return EXIT_FAILURE;
        default: usage(argv[0]); return EXIT_FAILURE;
        }
    }

    mpz_t n, d;
    mpz_inits(n, d, NULL);

    pvfile = fopen(pv_file_name, "r");
    if (pvfile == NULL) {
        printf("ERROR OPENING %s.\n", pv_file_name);
        mpz_clears(n, d, NULL);
        return EXIT_FAILURE;
    }

    rsa_read_priv(n, d, pvfile);
    fclose(pvfile);

    if (v) {
        gmp_printf("n (%d bits) = %Zd\n"
                   "d (%d bits) = %Zd\n",
            mpz_sizeinbase(n, 2), n, mpz_sizeinbase(d, 2), d);
    }
    // add into error handling that u get random chars printed out
    rsa_decrypt_file(input, output, n, d);

    fclose(input);
    fclose(output);
    mpz_clears(n, d, NULL);
    return 0;
}
