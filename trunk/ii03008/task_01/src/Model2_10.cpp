#include "Model2_10.h"
#include <cmath>

Model2_10::Model2_10(double a_val, double b_val)
    : Model(a_val, "Model 2.10 (Hyperbolic Tangent Smoothing)[cite: 2]"), b_coef(b_val) {}

double Model2_10::calculateStep(double u_current, double u_prev1, double u_prev2) {
    (void)u_prev1;
    (void)u_prev2;
    double y_next = getA() * std::tanh(y_prev) + b_coef * std::pow(u_current, 3);
    y_prev = y_next;
    return y_next;
}

void Model2_10::reset() {
    y_prev = 0.0;
}