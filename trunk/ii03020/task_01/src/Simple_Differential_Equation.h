#pragma once
#include "Model.h"
#include "InputGenerator.h"

class Simple_Differential_Equation : public Model {
    double b;
    double dt;
    double y = 0;
    double last_u = 0;
    InputGenerator input;
public:
    Simple_Differential_Equation(double b, double dt,
                                 int type, double u_const);
    double NextStep() override;
    double GetY()     const override;
    double GetLastU() const override;
};