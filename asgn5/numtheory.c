#include "numtheory.h"
#include "randstate.h"

// Computes the greatest common divisor of a and b.
// Code derived from asgn5 pdf.
// Takes in arguments d, a, and b.
//
// Stores the gcd of a and b into d.
// Returns nothing.
void gcd(mpz_t d, mpz_t a, mpz_t b) {
    // Initialize temp variable && make sure not to modify original values.
    mpz_t temp, temp_a, temp_b;
    mpz_inits(temp, temp_a, temp_b, NULL);
    mpz_set(temp_a, a);
    mpz_set(temp_b, b);
    mpz_set_ui(temp, 0);

    // b != 0
    while (mpz_cmp_ui(temp_b, 0) != 0) {
        // Perform a swap
        mpz_set(temp, temp_b);
        mpz_mod(temp_b, temp_a, temp_b);
        mpz_set(temp_a, temp);
    }

    // d = a
    mpz_set(d, temp_a);

    // clear the memory used
    mpz_clears(temp, temp_a, temp_b, NULL);
    return;
}

// Computes the modular inverse of a and n.
// Code derived from asgn5 pdf.
// Takes in arguments i, a, and n.
//
// Stores the modular inverse of a and n into i.
// Returns nothing.
void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    // Initialize variables
    mpz_t r, r_pri, t, t_pri, temp, q;
    mpz_inits(r, r_pri, t, t_pri, temp, q, NULL);

    // r = n, r_pri = a, t = 0, t_pri = 1
    mpz_set(r, n);
    mpz_set(r_pri, a);
    mpz_set_ui(t, 0);
    mpz_set_ui(t_pri, 1);

    // r_pri != 0
    while (mpz_cmp_ui(r_pri, 0) != 0) {
        // q = r / r_pri
        mpz_fdiv_q(q, r, r_pri);

        // perform swap on r and r_pri
        mpz_set(temp, r);
        mpz_set(r, r_pri);
        mpz_mul(r_pri, q, r_pri);
        mpz_sub(r_pri, temp, r_pri);

        // perform swap on t and t_pri
        mpz_set(temp, t);
        mpz_set(t, t_pri);
        mpz_mul(t_pri, q, t_pri);
        mpz_sub(t_pri, temp, t_pri);
    }

    // r > 1
    if (mpz_cmp_ui(r, 1) > 0) {
        // i = 0
        // clear the memory used
        mpz_set_ui(i, 0);
        mpz_clears(r, r_pri, t, t_pri, temp, q, NULL);
        return;
    }

    // t < 0
    if (mpz_cmp_ui(t, 0) < 0) {
        // t = t + n
        mpz_add(t, t, n);
    }

    // i = t
    // clear the memory used
    mpz_set(i, t);
    mpz_clears(r, r_pri, t, t_pri, temp, q, NULL);
    return;
}

// Performs a power mod.
// Code derived from asgn5 pdf.
// Takes in arguments out, base, exponent, and modulus.
//
// Stores the power mod of base, exponent, and modulus into out.
// Returns nothing.
void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus) {
    // Initialize variables p, v, and r.
    mpz_t p, v, temp_exponent;
    mpz_inits(p, v, temp_exponent, NULL);

    // v = 1, p = base, temp_exponent = exponent
    mpz_set_ui(v, 1);
    mpz_set(p, base);
    mpz_set(temp_exponent, exponent);

    // while (exponent > 0)
    while (mpz_cmp_ui(temp_exponent, 0) > 0) {
        // exponent is odd
        if (mpz_odd_p(temp_exponent) != 0) {
            // v = (v * p) % modulus
            mpz_mul(v, v, p);
            mpz_mod(v, v, modulus);
        }

        // p = (p * p) % modulus
        mpz_mul(p, p, p);
        mpz_mod(p, p, modulus);

        // exponent /= 2
        mpz_fdiv_q_ui(temp_exponent, temp_exponent, 2);
    }

    // return v
    // clear the memory used
    mpz_set(out, v);
    mpz_clears(v, p, temp_exponent, NULL);
    return;
}

// Checks to see if a number is prime using Miller-Rabin's primality test.
// Code derived from asgn 5 pdf.
// Takes in the number to check for primality n, and the number of iterations iters.
//
// Returns true if the n is prime, false if not.
bool is_prime(mpz_t n, uint64_t iters) {
    // Initialize variables
    mpz_t s, s_min, r, n_min, n_min4, a, y, j, two;
    mpz_inits(s, s_min, r, n_min, n_min4, a, y, j, two, NULL);

    // s = 0, n_min = n - 1, r = n_min, n_min4 = n - 3, two = 2
    mpz_sub_ui(n_min, n, 1);
    mpz_set(r, n_min);
    mpz_sub_ui(n_min4, n, 3);
    mpz_set_ui(two, 2);

    // n == 1 || n == 2 || n == 3
    if (mpz_cmp_ui(n, 1) == 0 || mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) {
        // clear the memory used
        mpz_clears(two, j, y, a, n_min4, n_min, r, s_min, s, NULL);
        return true;
    }

    // n is even || n is 0
    if (mpz_even_p(n) != 0 || mpz_cmp_ui(n, 0) == 0) {
        // clear the memory used
        mpz_clears(two, j, y, a, n_min4, n_min, r, s_min, s, NULL);
        return false;
    }

    // r is even
    while (mpz_even_p(r) != 0) {
        // r /= 2
        mpz_fdiv_q_ui(r, r, 2);
        // s += 1
        mpz_add_ui(s, s, 1);
    }
    // s_min = s - 1
    mpz_sub_ui(s_min, s, 1);

    // Miller-Rabin primality test
    for (uint64_t i = 1; i < iters; i++) {
        // a = random() % n_min4
        mpz_urandomm(a, state, n_min4);

        // a = a + 2
        mpz_add_ui(a, a, 2);

        // y = pow_mod(a, r, n)
        pow_mod(y, a, r, n);

        // y != 1 && y != n_min
        if ((mpz_cmp_ui(y, 1) != 0) && mpz_cmp(y, n_min) != 0) {
            // j = 1
            mpz_set_ui(j, 1);

            // j <= s - 1 && y != n_min
            while ((mpz_cmp(j, s_min) <= 0) && mpz_cmp(y, n_min) != 0) {
                // y = pow_mod(y, 2, n)
                pow_mod(y, y, two, n);

                // y = 1
                if (mpz_cmp_ui(y, 1) == 0) {
                    // clear the memory used
                    mpz_clears(two, j, y, a, n_min4, n_min, r, s_min, s, NULL);
                    return false;
                }

                // j += 1
                mpz_add_ui(j, j, 1);
            }

            // y != n_min
            if (mpz_cmp(y, n_min) != 0) {
                // clear the memory used
                mpz_clears(two, j, y, a, n_min4, n_min, r, s_min, s, NULL);
                return false;
            }
        }
    }

    // clear the memory used
    mpz_clears(two, j, y, a, n_min4, n_min, r, s_min, s, NULL);
    return true;
}

// Generates a prime number
// Code inspired through the asgn 5 pdf.
// Takes in arguments p, number of bits bits, number of iterations iters.
//
// Stores the prime number into argument p.
// Return nothing.
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    // Initialize the variables
    size_t size;
    mpz_t t;
    mpz_init(t);

    // t = random() % bits
    mpz_urandomb(t, state, bits + 1);
    // size = log2(t)
    size = mpz_sizeinbase(t, 2);

    // while t isn't a prime or size of t is less than bits
    while ((!is_prime(t, iters)) || (size < bits)) {
        // t = random() % bits
        mpz_urandomb(t, state, bits + 1);
        // size = log2(t)
        size = mpz_sizeinbase(t, 2);
    }

    // p = t
    mpz_set(p, t);
    // clear the memory used
    mpz_clear(t);
    return;
}
