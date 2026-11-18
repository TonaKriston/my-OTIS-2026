#pragma once

#include "model.h"

class Model19 : public Model {
private:
    double a1;
    double a2;
    double a3;
    double b1;

    double y = 0.0;
    double yPrev1 = 0.0;
    double yPrev2 = 0.0;

public:
    Model19(double coeffA1, double coeffA2, double coeffA3, double coeffB1);

    double nextStep(double u) override;
    std::string getName() const override;
};
