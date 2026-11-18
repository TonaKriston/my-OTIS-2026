#include "Model3_9.h"
#include <cmath>

Model3_9::Model3_9(double b_val, double dt_val, double initial_y)
    : Model(initial_y), b(b_val), dt(dt_val) {}

double Model3_9::step(double u) {
    double dy = b * std::tanh(u);
    double next_y = getY() + dy * dt;
    setY(next_y);
    return next_y;
}