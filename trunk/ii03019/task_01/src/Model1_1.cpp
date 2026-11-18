#include "Model1_1.h"

Model1_1::Model1_1(double a_val, double b_val, double initial_y)
    : Model(initial_y), a(a_val), b(b_val) {}

double Model1_1::step(double u) {
    double next_y = a * getY() + b * u;
    setY(next_y);
    return next_y;
}