#include "Model15.h"

Model15::Model15(double y0, double a1_, double a2_, double b_, int k_, int n)
    : y(y0), a1(a1_), a2(a2_), b(b_), k(k_), history(n + k + 2, y0) {
}

double Model15::step(double u, int tau) {
    double delayedY = (tau - k >= 0) ? history[tau - k] : history[0];

    double newY = a1 * y + a2 * delayedY + b * u;

    history[tau + 1] = newY;
    y = newY;

    return y;
}

double Model15::getY() const {
    return y;
}
