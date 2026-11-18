#pragma once
#include "Model.h"

class Model2_4 : public Model {
private:
    double a;
    double b;

public:
    explicit Model2_4(double a_val, double b_val, double initial_y = 0.0);
    double step(double u) override;
};