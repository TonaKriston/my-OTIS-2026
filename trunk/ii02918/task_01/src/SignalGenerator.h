#pragma once
#include <cmath>
#include <string>
//  SignalGenerator Ч абстрактный генератор входного воздействи€
class SignalGenerator
{
public:
    virtual ~SignalGenerator() = default;

    // «начение сигнала на шаге tau (нумераци€ начинаетс€ с 1)
    virtual double value(int tau) const = 0;

    // »м€ типа сигнала
    virtual std::string getName() const = 0;
};
//  StepSignal Ч ступенчатое воздействие: u = const
class StepSignal : public SignalGenerator
{
private:
    static constexpr double DEFAULT_LEVEL = 1.0;
    double level;

public:
    explicit StepSignal(double level_ = DEFAULT_LEVEL) : level(level_) {}

    double value(int /*tau*/) const override
    {
        return level;
    }

    std::string getName() const override
    {
        return "Step (u = const)";
    }
};
//  BurstSignal Ч импульсное воздействие: u_1 = 1, u_{tau>1} = 0
class BurstSignal : public SignalGenerator
{
public:
    double value(int tau) const override
    {
        return (tau == 1) ? 1.0 : 0.0;
    }

    std::string getName() const override
    {
        return "Burst (impulse at tau=1)";
    }
};
//  HarmonicSignal Ч гармоническое воздействие: u = sin(tau)
class HarmonicSignal : public SignalGenerator
{
public:
    double value(int tau) const override
    {
        return std::sin(static_cast<double>(tau));
    }

    std::string getName() const override
    {
        return "Harmonic (u = sin(tau))";
    }
};