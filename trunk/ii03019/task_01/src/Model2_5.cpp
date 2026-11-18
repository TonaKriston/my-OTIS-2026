#include "Model2_5.h"
#include <cmath>

Model2_5::Model2_5(double a_val, double b_val, double initial_y)
    : Model(initial_y), a(a_val), b(b_val) {}

double Model2_5::sign(double val) {
    if (val > 0.0) return 1.0;
    if (val < 0.0) return -1.0;
    return 0.0;
}

double Model2_5::step(double u) {
    double next_y = a * getY() + b * sign(u) * (1.0 - std::exp(-std::abs(u)));
    setY(next_y);
    return next_y;
}