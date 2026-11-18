#pragma once
#include "model.h"

class NonlinearModel2_9 : public Model {
public:
    NonlinearModel2_9(double a, double b);
    double step(double u) override;
    void reset() override;
    std::string getName() const override { return "Nonlinear 2.9"; }

private:
    double a_ = 0.0;
    double b_ = 0.0;
    double y_ = 0.0;
};