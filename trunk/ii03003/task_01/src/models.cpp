#include "models.hpp"

#include <stdexcept>

DelayedLinearModel::DelayedLinearModel(double a, double b, std::size_t delay)
    : a_(a), b_(b), delay_(delay) {
    if (delay < 1) throw std::invalid_argument("Delay k must be at least 1");
}

std::string DelayedLinearModel::id() const { return "model_1_2"; }
std::string DelayedLinearModel::title() const { return "Model 1.2 - Linear input delay"; }

double DelayedLinearModel::nextStep(const SimulationState& state) const {
    // Input before tau=0 is defined to be zero.
    const double delayedInput = state.step >= delay_
        ? state.inputs.at(state.step - delay_) : 0.0;
    return a_ * state.y + b_ * delayedInput;
}

PolynomialNonlinearModel::PolynomialNonlinearModel(double a1, double a2, double b)
    : a1_(a1), a2_(a2), b_(b) {}

std::string PolynomialNonlinearModel::id() const { return "model_2_6"; }
std::string PolynomialNonlinearModel::title() const { return "Model 2.6 - Polynomial nonlinear"; }

double PolynomialNonlinearModel::nextStep(const SimulationState& state) const {
    return a1_ * state.y * state.y * state.y
         - a2_ * state.yPrevious + b_ * state.u * state.u;
}

OffsetEulerModel::OffsetEulerModel(double a, double b) : a_(a), b_(b) {}
std::string OffsetEulerModel::id() const { return "model_3_10"; }
std::string OffsetEulerModel::title() const { return "Model 3.10 - Constant offset ODE / Euler"; }

double OffsetEulerModel::nextStep(const SimulationState& state) const {
    const double derivative = -a_ * state.y + b_ + state.u;
    return state.y + state.dt * derivative;
}
