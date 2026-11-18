#pragma once
#include "Model.h"
#include "InputGenerator.h"

class Linear_Model : public Model {
    double a;
    double b1;
    double b2;
    double b3;
    double y = 0;
    double u_t = 0;
    double u_t1 = 0;
    double u_t2 = 0;
    double last_u = 0;
    InputGenerator input;
public:
    Linear_Model(double a, double b1, double b2, double b3,
                 int type, double u_const);
    double NextStep() override;
    double GetY()     const override;
    double GetLastU() const override;
};