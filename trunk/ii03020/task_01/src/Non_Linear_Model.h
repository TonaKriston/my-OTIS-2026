#pragma once
#include "Model.h"
#include "InputGenerator.h"

class Non_linear_Model : public Model {
    double a;
    double b;
    double c;
    double d;
    double y = 0;
    double y_prev = 0;
    double u_t = 0;
    double u_t1 = 0;
    double last_u = 0;
    InputGenerator input;
public:
    Non_linear_Model(double a, double b, double c, double d,
                     int type, double u_const);
    double NextStep() override;
    double GetY()     const override;
    double GetLastU() const override;
};