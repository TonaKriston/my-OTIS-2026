#include "DiffEqModel.h"

DiffEqModel::DiffEqModel(double _a, double _dt) : a(_a), dt(_dt) {}

double DiffEqModel::step(double u) {
    double y_next = y_t + dt * (-a * y_t * y_t);
    y_t = y_next;
    return y_next;
}

void DiffEqModel::reset() { y_t = 0; }