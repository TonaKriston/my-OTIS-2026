#pragma once

#include <string>

class InputSignal
{
public:
    virtual ~InputSignal() = default;
    virtual double getValue(int tau) const = 0;
    virtual std::string getLabel() const = 0;
};

class ConstantSignal : public InputSignal
{
private:
    double level;

public:
    explicit ConstantSignal(double level_ = 1.0) : level(level_) {}

    double getValue(int tau) const override;
    std::string getLabel() const override;
};

class PulseSignal : public InputSignal
{
public:
    double getValue(int tau) const override;
    std::string getLabel() const override;
};

class SineSignal : public InputSignal
{
public:
    double getValue(int tau) const override;
    std::string getLabel() const override;
};