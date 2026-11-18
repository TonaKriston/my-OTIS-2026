#include "model19.h"

Model19::Model19(double coeffA1, double coeffA2, double coeffA3, double coeffB1)
    : a1(coeffA1), a2(coeffA2), a3(coeffA3), b1(coeffB1) {
}

double Model19::nextStep(double u) {
    const double yNext = a1 * y
                       + a2 * yPrev1
                       + a3 * yPrev2
                       + b1 * u;

    yPrev2 = yPrev1;
    yPrev1 = y;
    y = yNext;

    return y;
}

std::string Model19::getName() const {
    return "Model 1.9";
}
