#pragma once

#include "model.h"

class Model23 : public Model {
private:
    double a;
    double b;
    double delta;

    double y = 0.0;

public:
    Model23(double coeffA, double coeffB, double deadZone);

    double nextStep(double u) override;
    std::string getName() const override;
};
