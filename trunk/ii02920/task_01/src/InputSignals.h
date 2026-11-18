#pragma once

#include <cmath>
#include <string>

// Все сигналы начинаются с tau = 0. До начала моделирования u = 0.
class InputSignal {
public:
    virtual ~InputSignal() = default;
    virtual double value(int tau) const = 0;
    virtual std::string name() const = 0;
};

class StepInput : public InputSignal {
private:
    double amplitude;

public:
    explicit StepInput(double amplitude = 1.0) : amplitude(amplitude) {
    }

    double value(int tau) const override {
        return tau < 0 ? 0.0 : amplitude;
    }

    std::string name() const override {
        return "Step";
    }
};

class ImpulseInput : public InputSignal {
public:
    double value(int tau) const override {
        return tau == 0 ? 1.0 : 0.0;
    }

    std::string name() const override {
        return "Impulse";
    }
};

class HarmonicInput : public InputSignal {
public:
    double value(int tau) const override {
        return tau < 0 ? 0.0 : std::sin(static_cast<double>(tau));
    }

    std::string name() const override {
        return "Harmonic";
    }
};
