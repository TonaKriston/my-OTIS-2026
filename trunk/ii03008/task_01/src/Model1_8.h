#pragma once
#include "Model.h"

class Model1_8 : public Model {
private:
    double a1 = 0.0;
    double a2 = 0.0;
    double b1 = 0.0;
    double b2 = 0.0;
    double y_prev1 = 0.0;
    double y_prev2 = 0.0;

public:
    Model1_8(double a1_val, double a2_val, double b1_val, double b2_val);
    double calculateStep(double u_current, double u_prev1, double u_prev2) override;
    void reset() override;
};