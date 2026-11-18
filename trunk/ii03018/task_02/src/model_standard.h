#pragma once

class ISignalGenerator {
public:
    virtual ~ISignalGenerator() = default;
    virtual double generate(int tau) const = 0;
};

class model_standard {
public:
    virtual ~model_standard() = default;
    virtual double nextStep(double u) = 0;
    virtual void reset() = 0;
};