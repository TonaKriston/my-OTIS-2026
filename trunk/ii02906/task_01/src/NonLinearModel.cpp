#include "NonLinearModel.h"
#include <cmath>

NonLinearModel::NonLinearModel(double _a, double _b) : a(_a), b(_b) {}

double NonLinearModel::step(double u) {
    double y_next = a * std::tanh(y_t) + b * std::pow(u, 3);
    y_t = y_next;
    return y_next;
}

void NonLinearModel::reset() { y_t = 0; }