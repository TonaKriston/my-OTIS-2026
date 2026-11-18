#pragma once

#include "model.h"

class Model37 : public Model {
private:
    double a;
    double b;
    double dt;

    double y = 0.0;

public:
    Model37(double coeffA, double coeffB, double timeStep);

    double nextStep(double u) override;
    std::string getName() const override;
};
