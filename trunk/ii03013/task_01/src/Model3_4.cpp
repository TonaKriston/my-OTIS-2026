#include "Model3_4.h"

Model3_4::Model3_4(double a)
    : a(a) {
}

double Model3_4::next(
    double y,
    double u,
    double delayedU,
    double dt
) const {
    (void)u;
    (void)delayedU;

    return y - dt * a * y * y;
}

std::string Model3_4::name() const {
    return "Differential Model 3.4";
}