#include "model23.h"

Model23::Model23(double coeffA, double coeffB, double deadZone)
    : a(coeffA), b(coeffB), delta(deadZone) {
}

double Model23::nextStep(double u) {
    double uNew = 0.0;

    if (u > delta) {
        uNew = u - delta;
    }
    else if (u < -delta) {
        uNew = u + delta;
    }

    y = a * y + b * uNew;

    return y;
}

std::string Model23::getName() const {
    return "Model 2.3";
}
