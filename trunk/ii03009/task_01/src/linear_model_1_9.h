#pragma once
#include "model.h"

class LinearModel1_9 : public Model {
public:
    LinearModel1_9(double a1, double a2, double a3, double b1);
    double step(double u) override;
    void reset() override;
    std::string getName() const override { return "Linear 1.9"; }

private:
    double a1_ = 0.0;
    double a2_ = 0.0;
    double a3_ = 0.0;
    double b1_ = 0.0;
    double y_prev1_ = 0.0; 
    double y_prev2_ = 0.0; 
    double y_ = 0.0;
};