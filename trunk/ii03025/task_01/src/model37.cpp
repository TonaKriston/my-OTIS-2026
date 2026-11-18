#include "model37.h"

#include <cmath>

Model37::Model37(double coeffA, double coeffB, double timeStep)
    : a(coeffA), b(coeffB), dt(timeStep) {
}

double Model37::nextStep(double u) {
    y = y + dt * (-std::exp(a) * y + b * u);

    return y;
}

std::string Model37::getName() const {
    return "Model 3.7";
}
