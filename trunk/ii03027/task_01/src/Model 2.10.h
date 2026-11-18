#pragma once
#include "Model.h"
#include <cmath>

class NonlinearModel2_10 : public Model {
private:
    double a = 0.0;
    double b = 0.0;
    double y_t = 0.0;

public:
    NonlinearModel2_10(double _a, double _b) : a(_a), b(_b) {}

    void reset() override { 
        y_t = 0.0; 
    }

    double step(double u) override {
        y_t = a * std::tanh(y_t) + b * std::pow(u, 3);
        return y_t;
    }
};