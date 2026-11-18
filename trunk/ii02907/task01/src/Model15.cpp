#include "Model15.h"
#include <algorithm>
#include <stdexcept>

Model15::Model15(double a1, double a2, double b, int delay)
    : a1_(a1), a2_(a2), b_(b), delay_(delay) {
    if (delay_ < 1) throw std::invalid_argument("Model 1.5 delay k must be >= 1");
}
std::string Model15::name() const { return "Model 1.5"; }

double Model15::calculateNext(double u, int, const std::vector<double>& history,
                              double) const {
    const double current = previousY(history, 0);
    const double delayed = previousY(history, delay_);
    return a1_ * current + a2_ * delayed + b_ * u;
}
