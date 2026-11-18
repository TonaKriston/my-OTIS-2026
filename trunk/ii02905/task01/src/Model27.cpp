#include "Model27.h"

Model27::Model27(double y0, double a_, double b_, double epsilon_)
    : y(y0), a(a_), b(b_), epsilon(epsilon_) {
}

double Model27::relay(double u) {
    if (u > epsilon)
        relayState = 1;
    else if (u < -epsilon)
        relayState = -1;

    return relayState;
}

double Model27::step(double u, int tau) {
    (void)tau;

    y = a * y + b * relay(u);

    return y;
}

double Model27::getY() const {
    return y;
}
