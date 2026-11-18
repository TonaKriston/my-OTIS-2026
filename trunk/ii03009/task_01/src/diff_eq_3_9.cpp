#include "diff_eq_3_9.h"
#include <cmath>

DiffEq3_9::DiffEq3_9(double b, double dt)
    : b_(b), dt_(dt) {}

double DiffEq3_9::step(double u) {
    double dydt = b_ * std::tanh(u);
    
    y_ = y_ + dt_ * dydt;
    return y_;
}

void DiffEq3_9::reset() {
    y_ = 0.0;
}