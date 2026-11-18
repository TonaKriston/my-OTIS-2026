#pragma once
#include "Model.h"

class DiffEqModel3_2 : public Model {
private:
    double b;
    double dt;
    double y_t = 0.0;
public:
    DiffEqModel3_2(double _b, double _dt);
    void reset() override;
    double step(double u) override;
};