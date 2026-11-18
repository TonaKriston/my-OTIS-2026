#include "model11.h"

Model11::Model11(double a, double b) : a(a), b(b), y_prev(0.0) {}

std::string Model11::getName() const {
    return "Model 11 - Линейная система";
}

void Model11::reset() {
    y_prev = 0.0;
}

double Model11::nextStep(double u, double dt) {
    double y_next = a * y_prev + b * u;
    y_prev = y_next; 
    return y_next;
}