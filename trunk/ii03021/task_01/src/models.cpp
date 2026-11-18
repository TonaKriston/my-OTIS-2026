#include "models.hpp"
#include <cmath>
#include <stdexcept>

namespace otis {

DelayedControlModel::DelayedControlModel(double a, double b, int delay)
    : a_(a), b_(b), delay_(delay) {
    if (delay < 1) throw std::invalid_argument("Delay k must be >= 1.");
}

double DelayedControlModel::step(double input, int tau) {
    inputs_.push_back(input);
    const int sourceTau = tau - delay_;
    const double delayedInput = sourceTau >= 0
        ? inputs_.at(static_cast<std::size_t>(sourceTau)) : 0.0;
    y_ = a_ * y_ + b_ * delayedInput;
    return y_;
}
void DelayedControlModel::reset() { y_ = 0.0; inputs_.clear(); }
const char* DelayedControlModel::name() const { return "Linear model 1.2"; }

LogarithmicInteractionModel::LogarithmicInteractionModel(double a, double b)
    : a_(a), b_(b) {}

double LogarithmicInteractionModel::step(double input, int) {
    y_ = a_ * y_ * std::cos(y_) + b_ * std::log1p(std::abs(input));
    return y_;
}
void LogarithmicInteractionModel::reset() { y_ = 0.0; }
const char* LogarithmicInteractionModel::name() const { return "Nonlinear model 2.4"; }

CubicDifferentialModel::CubicDifferentialModel(double a, double b, double dt)
    : a_(a), b_(b), dt_(dt) {
    if (dt <= 0.0) throw std::invalid_argument("Time step dt must be > 0.");
}

double CubicDifferentialModel::step(double input, int) {
    // Explicit Euler: y(t + dt) = y(t) + dt * (a*y(t)^3 + b*u(t)).
    y_ += dt_ * (a_ * y_ * y_ * y_ + b_ * input);
    return y_;
}
void CubicDifferentialModel::reset() { y_ = 0.0; }
const char* CubicDifferentialModel::name() const { return "Differential model 3.6"; }

} // namespace otis
