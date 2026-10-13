#include "Models.h"
#include <iostream>
#include <cmath>

Model1_7::Model1_7(double a_val, double b1_val, double b2_val, double b3_val)
    : a(a_val), b1(b1_val), b2(b2_val), b3(b3_val) {
    if (std::abs(a) >= 1.0) {
        std::cout << "[WARNING] Model 1.7: System is UNSTABLE (|a| >= 1)\n";
    }
}

void Model1_7::reset() {
    y = 0.0;
    u_prev1 = 0.0;
    u_prev2 = 0.0;
}

double Model1_7::nextStep(double u) {
    y = a * y + b1 * u + b2 * u_prev1 + b3 * u_prev2;
    u_prev2 = u_prev1;
    u_prev1 = u;
    return y;
}

Model2_9::Model2_9(double a_val, double b_val)
    : a(a_val), b(b_val) {}

void Model2_9::reset() {
    y = 0.0;
}

double Model2_9::nextStep(double u) {
    double sign = 0.0;
    if (u > 0.0) {
        sign = 1.0;
    }
    else if (u < 0.0) {
        sign = -1.0;
    }

    y = a * y + b * std::sqrt(std::abs(u)) * sign;
    return y;
}

Model3_1::Model3_1(double a_val, double dt_val)
    : a(a_val), dt(dt_val) {}

void Model3_1::reset() {
    y = 1.0;
}

double Model3_1::nextStep(double /*u*/) {
    y = y - dt * a * y;
    return y;
}