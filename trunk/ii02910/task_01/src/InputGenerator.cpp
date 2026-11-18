#include "InputGenerator.h"
#include <cmath>

double ConstantSignal::getValue(int tau) const
{
    (void)tau;
    return level;
}

std::string ConstantSignal::getLabel() const
{
    return "Constant (u = " + std::to_string(level) + ")";
}

double PulseSignal::getValue(int tau) const
{
    return (tau == 1) ? 1.0 : 0.0;
}

std::string PulseSignal::getLabel() const
{
    return "Pulse (u_1 = 1, u_tau>1 = 0)";
}

double SineSignal::getValue(int tau) const
{
    return std::sin(static_cast<double>(tau));
}

std::string SineSignal::getLabel() const
{
    return "Sine (u_tau = sin(tau))";
}