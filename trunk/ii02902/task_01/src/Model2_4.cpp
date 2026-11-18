#include "Model2_4.h"
#include <cmath>

Model2_4::Model2_4(double a_val, double b_val, double initial_y)
    : Model(initial_y), a(a_val), b(b_val) {}

double Model2_4::step(double u) {
    const double current_y = getY();
    const double next_y = a * current_y * std::cos(current_y) + b * std::log(1.0 + std::abs(u));
    setY(next_y);
    return next_y;
}