#include "Model1_10.h"

Model1_10::Model1_10(double a1, double b1, double b2)
    : a1(a1), b1(b1), b2(b2) {
}

double Model1_10::next(
    double y,
    double u,
    double delayedU,
    double dt
) const {
    (void)dt;

    return a1 * y + b1 * u + b2 * delayedU;
}

std::string Model1_10::name() const {
    return "Linear Model 1.10";
}

bool Model1_10::isStable() const {
    return a1 > -1.0 && a1 < 1.0;
}

std::string Model1_10::stabilityWarning() const {
    if (!isStable()) {
        return "Warning: the model may be unstable because |a1| >= 1.";
    }

    return "";
}