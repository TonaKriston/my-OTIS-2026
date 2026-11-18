#pragma once
#include "Model.h"
#include <vector>

class Model1_2 : public Model {
private:
    double a;
    double b;
    int delay_k;
    std::vector<double> u_history;

public:
    explicit Model1_2(double a_val, double b_val, int k_val, double initial_y = 0.0);
    double step(double u) override;
    void reset(double initial_y = 0.0) override;
};