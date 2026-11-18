#include "Model39.h"
#include <cmath>
#include <stdexcept>

Model39::Model39(double b) : b_(b) {}
std::string Model39::name() const { return "Model 3.9"; }

double Model39::calculateNext(double u, int, const std::vector<double>& history,
                              double dt) const {
    if (dt <= 0.0) throw std::invalid_argument("dt must be > 0");
    return previousY(history, 0) + dt * b_ * std::tanh(u);
}
