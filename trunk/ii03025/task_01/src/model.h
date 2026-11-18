#pragma once

#include <string>

class Model {
public:
    Model() = default;
    virtual ~Model() = default;

    virtual double nextStep(double u) = 0;

    virtual std::string getName() const = 0;
};
