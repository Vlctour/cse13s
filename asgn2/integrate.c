#include "functions.h"

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Prints out a help page to show how to use the program.
// Followed the provided ./integrate usage page.
void usage(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "    Performs numerical integrations using composite Simpson's 1/3 rule.\n"
                    "\n"
                    "USAGE\n"
                    "\n"
                    "./integrate [-abcdefghijn:p:q:H]\n"
                    "\n"
                    "More info on the options\n"
                    "-a                sqrt(1 - x^4)\n"
                    "-b                1/log(x)\n"
                    "-c                exp(-x^2)\n"
                    "-d                sin(x^2)\n"
                    "-e                cos(x^2)\n"
                    "-f                log(log(x))\n"
                    "-g                sin(x)/x\n"
                    "-h                exp(-x)/x\n"
                    "-i                exp(exp(x))\n"
                    "-j                sqrt(sin^2(x) + cos^2(x))\n"
                    "-n partitions     Max num. of partitions\n"
                    "-p low            Set low end of integral interval\n"
                    "-q high           Set high end of integral interval\n"
                    "-H                Display program sysnopsis and usage\n");
}

// Approximates an integral over a specified bound using the Simpson's 1/3 rule.
// Function derived by asgn2.pdf Simpson's 3/8 rule.
// Takes in a function via the function pointer *fun.
// Takes in a lower bound a.
// Takes in an upper bound b.
// Takes in a partition number n.
//
// Returns the sum of the simpson's 1/3 rule.
double simpsons(double (*fun)(double), double a, double b, uint32_t n) {
    int nn = n * 1;
    double h, sum;
    h = (b - a) / n;
    sum = fun(a) + fun(b);

    for (int i = 1; i < nn; i++) {
        if (i % 2 == 0) {
            sum += 2 * fun(a + i * h);
        } else {
            sum += 4 * fun(a + i * h);
        }
    }

    sum *= h / 3;
    return sum;
}

// Takes in arguments specified during execution of the executable file.
// Prints out the value of the integral of some function along side the partition number.
// Prints out usage page if nothing is specified, or invalid specifications.
// Followed a similar example that eugene did during section for the getopt.
//
// Returns 0.
int main(int argc, char **argv) {
    int opt = 0, select, sel_name;
    double n_par = 100.0;
    double p_val, q_val;
    double (*fun[])(double) = { a, b, c, d, e, f, g, h, i, j };
    bool on = false;
    char *names[] = { "sqrt(1 - x^4)", "1/log(x)", "exp(-x^2)", "sin(x^2)", "cos(x^2)",
        "log(log(x))", "sin(x)/x", "exp(-x)/x", "exp(exp(x))", "sqrt(sin^2(x) + cos^2(x))" };

    while ((opt = getopt(argc, argv, "abcdefghijn:p:q:H")) != -1) {
        switch (opt) {
        case 'a':
            on = true;
            select = 0;
            sel_name = 0;
            break;
        case 'b':
            on = true;
            select = 1;
            sel_name = 1;
            break;
        case 'c':
            on = true;
            select = 2;
            sel_name = 2;
            break;
        case 'd':
            on = true;
            select = 3;
            sel_name = 3;
            break;
        case 'e':
            on = true;
            select = 4;
            sel_name = 4;
            break;
        case 'f':
            on = true;
            select = 5;
            sel_name = 5;
            break;
        case 'g':
            on = true;
            select = 6;
            sel_name = 6;
            break;
        case 'h':
            on = true;
            select = 7;
            sel_name = 7;
            break;
        case 'i':
            on = true;
            select = 8;
            sel_name = 8;
            break;
        case 'j':
            on = true;
            select = 9;
            sel_name = 9;
            break;
        case 'n': n_par = strtod(optarg, NULL); break;
        case 'p': p_val = strtod(optarg, NULL); break;
        case 'q': q_val = strtod(optarg, NULL); break;
        case 'H': usage(); break;
        default: usage(); return EXIT_FAILURE;
        }
    }

    if (on) {
        printf("%s,%f,%f,%0.0f\n", names[sel_name], p_val, q_val, n_par);
        for (int j = 2; j <= n_par; j += 2) {
            printf("%d,%0.15f\n", j, simpsons(fun[select], p_val, q_val, j));
        }
    } else {
        usage();
    }

    return 0;
}
