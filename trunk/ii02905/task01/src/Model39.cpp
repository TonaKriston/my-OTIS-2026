#include "Model39.h"

Model39::Model39(double y0, double b_, double dt_)
    : y(y0), b(b_), dt(dt_) {
}

double Model39::step(double u, int tau) {
    (void)tau;

    y = y + dt * b * std::tanh(u);

    return y;
}

double Model39::getY() const {
    return y;
}
