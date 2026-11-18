#include "model36.h"

Model36::Model36(double a, double b) : a(a), b(b), y(0.0) {}

std::string Model36::getName() const {
    return "Model 36 - Дифференциальная система";
}

void Model36::reset() {
    y = 0.0;
}

double Model36::nextStep(double u, double dt) {
    double dy = a * (y * y * y) + b * u;
    y = y + dt * dy;
    return y;
}