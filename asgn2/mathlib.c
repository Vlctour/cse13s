#include "mathlib.h"

#include <math.h>
#include <stdio.h>

#define PI 3.14159265358979323846

// Computes the modulus of two decimal numbers
// Code derived from Prof. Long's python code for modulus, found on discord
// Takes in two parameters n and m
//
// Returns the negative absolute value of r is it's less than 0
// Otherwise, it returns absolute value of r
double modu(double n, double m) {
    double r;
    int temp_tot;
    temp_tot = n / m;
    r = n - (m * temp_tot);

    if (n < 0.0) {
        return -1 * Abs(r);
    } else {
        return Abs(r);
    }
}

// Computes any e^x given a double value x
// Code derived from asgn2.pdf
// Takes in one parameter x
double Exp(double x) {
    double trm = 1.0, sum = trm, i = 1.0, epsilon = EPSILON;

    while (trm > epsilon) {
        trm = trm * (Abs(x) / i);
        sum += trm;
        i += 1;
    }

    if (x <= 0) {
        return (1 / sum);
    } else {
        return sum;
    }
}

// Computes any sin(x) where x is a double value
// Code derived from asgn2.pdf
// Takes in one parameter x
//
// If value is bigger than 2pi, I scaled the value down before calculating
double Sin(double x) {
    double s = 1.0, v = x, t = x, k = 3.0, epsilon = EPSILON;

    if (x > 2 * PI) {
        x = modu(x, 2 * PI);
        v = x;
        t = x;
    } else if (x < -2 * PI) {
        x = modu(x, -2 * PI);
        v = x;
        t = x;
    }

    while (Abs(t) > epsilon) {
        t = (t * (x * x)) / ((k - 1) * k);
        s = -s;
        v += s * t;
        k += 2.0;
    }

    return v;
}

// Computes any cos(x) where x is a double value
// Code derived partly from sin(x) on asgn1.pdf
// Takes in one parameter x
//
// If value is bigger than 2pi, I scaled the value down before calculating
double Cos(double x) {
    double s = 1.0, v = 1.0, t = 1.0, k = 2.0, epsilon = EPSILON;

    if (x > 2 * PI) {
        x = modu(x, 2 * PI);
        v = 1.0;
        t = 1.0;
    } else if (x < -2 * PI) {
        x = modu(x, -2 * PI);
        v = 1.0;
        t = 1.0;
    }

    while (Abs(t) > epsilon) {
        t = (t * (x * x)) / ((k - 1) * k);
        s = -s;
        v += s * t;
        k += 2.0;
    }

    return v;
}

// Computes any sqrt(x) where x is a double value
// Code derived from asgn2.pdf
// Takes in one parameter x
//
// If value is bigger than 1, I scaled the value down before calculating
double Sqrt(double x) {
    double z = 0.0, y = 1.0, f = 1.0, epsilon = EPSILON;

    if (x < 0.0) {
        return -NAN;
    }

    while (x > 1.0) {
        x /= 4.0;
        f *= 2.0;
    }

    while (Abs(y - z) > epsilon) {
        z = y;
        y = 0.5 * (z + x / z);
    }

    return y * f;
}

// Computes any log(x) where x is a double value
// Code derived from asgn2.pdf
// Takes in one parameter x
//
// If value is bigger than e, I scaled the value down before calculating
double Log(double x) {
    double y = 1.0, p = Exp(y), f = 0.0, epsilon = EPSILON, e = 2.7182818284590455;

    if (x < 0) {
        return -NAN;
    } else if (x == 0) {
        return -INFINITY;
    }

    while (x > e) {
        x /= e;
        f += 1.0;
    }

    while (Abs(p - x) > epsilon) {
        y = y + x / p - 1;
        p = Exp(y);
    }

    return y + f;
}
