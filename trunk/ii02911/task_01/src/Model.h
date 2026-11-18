#pragma once

class Model
{
public:
    virtual ~Model() = default;

    virtual double calculateNext(
        const double currentValue,
        const double previousValue,
        const double secondPreviousValue,
        const double inputValue,
        const double timeStep
    ) const = 0;

    virtual const char* getName() const = 0;
};
