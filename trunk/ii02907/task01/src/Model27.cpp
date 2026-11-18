#include "Model27.h"
#include <cmath>
#include <stdexcept>

Model27::Model27(double a, double b, double epsilon)
    : a_(a), b_(b), epsilon_(epsilon) {
    if (epsilon_ < 0.0) throw std::invalid_argument("epsilon must be >= 0");
}
std::string Model27::name() const { return "Model 2.7"; }

// Relay with hysteresis: state is held inside the dead band.
// Outside the band, relay becomes +1 or -1. At the initial step,
// the relay state is selected from the current input.
double Model27::calculateNext(double u, int, const std::vector<double>& history,
                              double) const {
    const double y = previousY(history, 0);
    double relay = 0.0;
    if (u > epsilon_) relay = 1.0;
    else if (u < -epsilon_) relay = -1.0;
    else if (y > epsilon_) relay = 1.0;
    else if (y < -epsilon_) relay = -1.0;
    return a_ * y + b_ * relay;
}
