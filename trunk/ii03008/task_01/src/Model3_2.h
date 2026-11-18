#pragma once
#include "Model.h"

class Model3_2 : public Model {
private:
    double b_coef = 0.0;
    double y_prev = 0.0;

public:
    explicit Model3_2(double b_val);
    double calculateStep(double u_current, double u_prev1, double u_prev2) override;
    void reset() override;
};