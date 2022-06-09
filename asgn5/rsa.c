#include <stdio.h>
#include <stdlib.h>
#include "randstate.h"
#include "rsa.h"
#include "numtheory.h"

// Compute the least common multiple of two numbers.
// Code derived from asgn 5 pdf.
// Takes in arguments d, a, b.
//
// Stores the least common multiple between a and b in d.
// Returns nothing.
void lcm(mpz_t d, mpz_t a, mpz_t b) {
    // Initialize variables
    mpz_t a_temp, b_temp;
    mpz_inits(a_temp, b_temp, NULL);

    // a_temp = a, b_temp = b
    mpz_set(a_temp, a);
    mpz_set(b_temp, b);

    // check if a/b are negative, if so, set them to positive
    if (mpz_cmp_ui(a_temp, 0) < 0) {
        mpz_mul_ui(a_temp, a_temp, -1);
    }
    if (mpz_cmp_ui(b_temp, 0) < 0) {
        mpz_mul_ui(b_temp, b_temp, -1);
    }

    // | a * b | / gcd(a, b)
    gcd(d, a_temp, b_temp);
    mpz_fdiv_q(d, b_temp, d);
    mpz_mul(d, a_temp, d);

    // clears the memory used
    mpz_clears(a_temp, b_temp, NULL);
    return;
}

// Makes parts of the RSA public key.
// Code influenced by asgn5 pdf specifications.
// Takes in arguments p, q, n, e, nbits, and iters.
//
// Stores the prime numbers in p/q, the product of p/q in n, and the exponent in e.
// Returns nothing.
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    // Initialize variables
    mpz_t n_pri, temp_e, p_min, q_min;
    mpz_inits(n_pri, temp_e, p_min, q_min, NULL);
    uint64_t random_bits;
    size_t n_cmp = 0;

    while (n_cmp < nbits) {
        // Generate a random number in the range of [nbits/4, 3*nbits/4]
        random_bits = (random() % (nbits / 2)) + (nbits / 4);
        // Make a prime number with the specified bits
        make_prime(p, random_bits, iters);

        // Make the other prime number using the remaining bits.
        random_bits = nbits - random_bits;
        make_prime(q, random_bits, iters);

        // n = p * q
        // Find the log2(n)
        mpz_mul(n, p, q);
        n_cmp = mpz_sizeinbase(n, 2);
    }

    // p_min = p - 1
    // q_min = q - 1
    mpz_sub_ui(p_min, p, 1);
    mpz_sub_ui(q_min, q, 1);

    // n_pri = lcm(p_min, q_min)
    lcm(n_pri, p_min, q_min);

    // random number from [0, nbits - 1]
    // temp_e = gcd(e, n_pri)
    mpz_urandomb(e, state, nbits);
    gcd(temp_e, e, n_pri);

    // temp_e != 1
    while (mpz_cmp_ui(temp_e, 1) != 0) {
        // random number from [0, nbits - 1]
        // temp_e = gcd(e, n_pri)
        mpz_urandomb(e, state, nbits);
        gcd(temp_e, e, n_pri);
    }

    // clear the memory used
    mpz_clears(n_pri, temp_e, p_min, q_min, NULL);
    return;
}

// Write the RSA key to a file.
// Code influenced by asgn5 specifications.
// Takes in arguments n, e, s, username, pbfile.
//
// Prints the variables n, e, s, and username to pbfile.
// Returns nothing.
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    fprintf(pbfile, "%s\n", username);
    return;
}

// Read a RSA key from a file.
// Code influenced by asgn5 specifications.
// Takes in arguments n, e, s, username, pbfile.
//
// Scans in stuff from the pbfile and stores it in the variables n, e, s, and username.
// Return nothing.
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);
    fscanf(pbfile, "%s\n", username);
    return;
}

// Creates a RSA private key.
// Code influenced by asgn5 specifications.
// Takes in arguments d, e, p, q.
//
// Stores the values into d and e.
// Returns nothing.
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    // Initialize variables
    mpz_t p_min, q_min, n_pri;
    mpz_inits(p_min, q_min, n_pri, NULL);

    // p_min = p - 1, q_min = q - 1, n_pri = lcm(p_min, q_min)
    mpz_sub_ui(p_min, p, 1);
    mpz_sub_ui(q_min, q, 1);
    lcm(n_pri, p_min, q_min);

    // d = mod_inverse(e, n_pri)
    mod_inverse(d, e, n_pri);

    // clear the memory used
    mpz_clears(p_min, q_min, n_pri, NULL);
    return;
}

// Write the private RSA key to a file.
// Code influenced by asgn5 specifications.
// Takes in arguments n, d, pvfile.
//
// Prints the arguments n and d into a pvfile.
// Return nothing.
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
    return;
}

// Read the private RSA key from a file.
// Code influenced by asgn5 specifications.
// Takes in arguments n, d, and pvfile.
//
// Stores the values read from the pv file into n and d.
// Returns nothing.
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", n);
    gmp_fscanf(pvfile, "%Zx\n", d);
    return;
}

// Performs RSA encryption.
// Code influenced by asgn5 specifications.
// Takes in arguments c, m, e, n.
//
// Stores encrypted message into c.
// Returns nothing.
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n);
    return;
}

// Performs RSA encryption on a file.
// Code derived from asgn5 pdf.
// Takes in arguments infile, outfile, n, e.
//
// Reads from an infile, encrypts the message, and passes the message into outfile.
// Returns nothing.
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    // Initialize variables, Allocate space on the heap for the array
    size_t bytes, size = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint8_t *arr = calloc(size, sizeof(uint8_t));
    mpz_t m, mes;
    mpz_inits(m, mes, NULL);

    // Allocate 0xFF to the 0th index of the array
    arr[0] = 0xFF;

    // While there are still unproccessed bytes in infile
    while ((bytes = fread(arr + 1, sizeof(uint8_t), size - 1, infile)) > 0) {
        // Turn the array into an mpz_t and store in m
        mpz_import(m, bytes + 1, 1, sizeof(uint8_t), 1, 0, arr);

        // Encrypt the message
        rsa_encrypt(mes, m, e, n);

        // Store into outfile with trailing new line
        gmp_fprintf(outfile, "%Zx\n", mes);
    }

    // Clear the memory used on mpz and the array
    mpz_clears(m, mes, NULL);
    free(arr);
    return;
}

// Performs RSA decryption.
// Code influenced by asgn 5 pdf.
// Takes in arguments m, c, d, n.
//
// Stores decrypted message into m.
// Returns nothing.
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
    return;
}

// Perform RSA decryption on a file.
// Code derived from asgn5 pdf.
// Takes in argument infile, outfile, n, d.
//
// Reads from infile, decrypts message, and stores message into outfile.
// Returns nothing.
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    // Initialize variables, Allocate space on the heap for the array
    size_t bytes, size = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint8_t *brr = calloc(size, sizeof(uint8_t));
    mpz_t c, ces;
    mpz_inits(c, ces, NULL);

    // While infile has not reached the end of the file
    while ((gmp_fscanf(infile, "%Zx\n", c)) != EOF) {
        // decrypt the message
        rsa_decrypt(ces, c, d, n);

        // export the message
        mpz_export(brr, &bytes, 1, sizeof(uint8_t), 1, 0, ces);

        // write the message into the outfile
        fwrite(brr + 1, sizeof(uint8_t), bytes - 1, outfile);
    }

    // clear the memory used, clear the array we used
    mpz_clears(c, ces, NULL);
    free(brr);
    return;
}

// Performs RSA signing.
// Code influenced by asgn5 specifications.
// Takes in arguments s, m, d, n.
//
// Stores results in variable s.
// Return nothing.
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n);
    return;
}

// Performs RSA verification.
// Code influenced by asgn5 specifications.
// Takes in argument m, s, e, n.
//
// Returns true if signature s is equal to the message m, otherwise false.
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    // Initialize variables
    mpz_t temp;
    mpz_init(temp);
    pow_mod(temp, s, e, n);

    // temp == m
    if (mpz_cmp(temp, m) == 0) {
        // clear the memory used
        mpz_clear(temp);
        return true;
    } else {
        // clear the memory used
        mpz_clear(temp);
        return false;
    }
}
