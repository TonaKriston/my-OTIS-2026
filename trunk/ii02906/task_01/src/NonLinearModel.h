#pragma once
#include "Model.h"

class NonLinearModel : public Model {
private:
    double a;
    double b;
    double y_t = 0;
public:
    NonLinearModel(double _a, double _b);
    double step(double u) override;
    void reset() override;
};
