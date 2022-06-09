#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <sys/stat.h>
#include <gmp.h>
#include "randstate.h"
#include "numtheory.h"
#include "rsa.h"

// Prints a helpful usage page for keygen.c.
void usage(char *exe) {
    fprintf(stderr,
        "Synopsis\n"
        "    Generates an RSA public/private key pair.\n"
        "\n"
        "Usage\n"
        "    %s [OPTIONS]\n"
        "\n"
        "Options\n"
        "    -h                Display program help and usage page.\n"
        "    -v                Display verbose program output.\n"
        "    -b bits           Minimum bits needed for public key n (default: 256).\n"
        "    -i interations    Miller-Rabin iterations for testing primes (default: 50).\n"
        "    -n pbfile         Public key file (default: rsa.pub).\n"
        "    -d pvfile         Private key file (default: rsa.priv).\n"
        "    -s seed           Random seed for testing.\n",
        exe);
}

// This program generates RSA keys for the user.
// Code is influenced by the specifications of the asgn5 pdf.
// Takes in user arguments and parses them through getopt.
//
// Returns 0.
int main(int argc, char **argv) {
    int64_t opt = 0, b = 256, i = 50, seed = time(NULL);
    bool v = false;
    FILE *pbfile, *pvfile;
    char *pb_file_name = "rsa.pub", *pv_file_name = "rsa.priv";
    char *username = getenv("USER");
    mpz_t u, p, q, n, e, d, s;
    mpz_inits(u, p, q, n, e, d, s, NULL);

    while ((opt = getopt(argc, argv, "b:i:n:d:s:vh")) != -1) {
        switch (opt) {
        case 'b': b = strtod(optarg, NULL); break;
        case 'i': i = strtod(optarg, NULL); break;
        case 'n': pb_file_name = optarg; break;
        case 'd': pv_file_name = optarg; break;
        case 's': seed = strtod(optarg, NULL); break;
        case 'v': v = true; break;
        case 'h': usage(argv[0]); return EXIT_FAILURE;
        default: usage(argv[0]); return EXIT_FAILURE;
        }
    }

    pbfile = fopen(pb_file_name, "w");
    pvfile = fopen(pv_file_name, "w");

    if (pbfile == NULL) {
        fprintf(stderr, "ERROR OPENING %s.\n", pb_file_name);
        return EXIT_FAILURE;
    }
    if (pvfile == NULL) {
        fprintf(stderr, "ERROR OPENING %s.\n", pv_file_name);
        return EXIT_FAILURE;
    }

    // Set perms to pvfile so that only the user can read and write to it.
    fchmod(fileno(pvfile), 0600);

    randstate_init(seed);

    rsa_make_pub(p, q, n, e, b, i);
    rsa_make_priv(d, e, p, q);

    mpz_set_str(u, username, 62);
    rsa_sign(s, u, d, n);

    rsa_write_pub(n, e, s, username, pbfile);
    rsa_write_priv(n, d, pvfile);

    if (v) {
        gmp_printf("user = %s\n"
                   "s (%d bits) = %Zd\n"
                   "p (%d bits) = %Zd\n"
                   "q (%d bits) = %Zd\n"
                   "n (%d bits) = %Zd\n"
                   "e (%d bits) = %Zd\n"
                   "d (%d bits) = %Zd\n",
            username, mpz_sizeinbase(s, 2), s, mpz_sizeinbase(p, 2), p, mpz_sizeinbase(q, 2), q,
            mpz_sizeinbase(n, 2), n, mpz_sizeinbase(e, 2), e, mpz_sizeinbase(d, 2), d);
    }

    fclose(pbfile);
    fclose(pvfile);
    mpz_clears(u, p, q, n, e, d, s, NULL);
    randstate_clear();
    return 0;
}
