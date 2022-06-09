#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "randstate.h"
#include "numtheory.h"
#include "rsa.h"

int main(void) {
    randstate_init(1);
    mpz_t p, q, n, e, d;
    mpz_inits(p, q, n, d, e, NULL);
    mpz_set_ui(p, 0);
    mpz_set_ui(q, 0);
    mpz_set_ui(n, 0);
    mpz_set_ui(e, 0);
    mpz_set_ui(d, 0);
    uint64_t nbits = 100, iters = 50;
    rsa_make_pub(p, q, n, e, nbits, iters);
    rsa_make_priv(d, e, p, q);

    mpz_t s, temp;
    mpz_inits(s, temp, NULL);
    mpz_set_str(temp, getenv("USER"), 62);
    rsa_sign(s, temp, d, n);
    FILE *pub = fopen("rsa.pub", "w");

    rsa_write_pub(n, e, s, getenv("USER"), pub);
    fclose(pub);

    FILE *priv = fopen("rsa.priv", "w");
    rsa_write_priv(n, d, priv);
    fclose(priv);
    gmp_printf("%Zd %Zd %Zd\n", p, q, n);
    if (is_prime(n, 50)) {
        printf("its prime\n");
    }
    mpz_t c, m;
    mpz_inits(c, m, NULL);
    mpz_set_ui(m, 10);
    rsa_encrypt(c, m, e, n);
    gmp_printf("c:%Zx\n", c);
    rsa_decrypt(m, c, d, n);
    gmp_printf("m:%Zd\n", m);

    FILE *infile = fopen("simple.txt", "r");
    FILE *outfile = fopen("simpler.txt", "w");
    rsa_encrypt_file(infile, outfile, n, e);
    fclose(infile);
    fclose(outfile);

    FILE *infile2 = fopen("simpler.txt", "r");
    FILE *outfile2 = fopen("simplest.txt", "w");
    rsa_decrypt_file(infile2, outfile2, n, d);
    fclose(infile2);
    fclose(outfile2);

    mpz_clears(p, q, n, e, d, c, m, s, temp, NULL);
    randstate_clear();

    return 0;
}
