#include "model28.h"
#include <cmath>

Model28::Model28(double a, double b, double c) : a(a), b(b), c(c), y(0.0), y_prev(0.0) {}

std::string Model28::getName() const {
    return "Model 28 - Нелинейная система";
}

void Model28::reset() {
    y = 0.0;
    y_prev = 0.0;
}

double Model28::nextStep(double u, double dt) {
    double y_next = a * y * (1.0 - y) + b * u + c * std::sin(y_prev * u);
    y_prev = y;
    y = y_next;
    return y_next;
}