#pragma once

#include "Model.h"

// dy/dt = -a*y + b + u
class Diff_3_10 : public Model {
private:
    double a;
    double b;
    double dt;
    double y = 0.0;

public:
    Diff_3_10(double a_, double b_, double dt_);
    void reset() override;
    double step(double u) override;
    const char* name() override;
};
