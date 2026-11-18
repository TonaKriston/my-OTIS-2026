#include "InputSignal.h"

#include <cmath>

StepSignal::StepSignal(double amplitude) : amplitude_(amplitude) {}

double StepSignal::value(int /*tau*/) const
{
    return amplitude_;
}

std::string StepSignal::name() const { return "ступенчатое"; }
std::string StepSignal::id() const { return "step"; }


ImpulseSignal::ImpulseSignal(double amplitude) : amplitude_(amplitude) {}

double ImpulseSignal::value(int tau) const
{
    return (tau == 0) ? amplitude_ : 0.0;
}

std::string ImpulseSignal::name() const { return "импульсное"; }
std::string ImpulseSignal::id() const { return "impulse"; }


HarmonicSignal::HarmonicSignal(double amplitude) : amplitude_(amplitude) {}

double HarmonicSignal::value(int tau) const
{
    return amplitude_ * std::sin(static_cast<double>(tau));
}

std::string HarmonicSignal::name() const { return "гармоническое"; }
std::string HarmonicSignal::id() const { return "harmonic"; }
