#pragma once
#include "Model.h"

class LinearModel13 : public Model {
private:
    double a1 = 0.0;
    double a2 = 0.0;
    double b = 0.0;
    double y_t = 0.0;
    double y_t_minus_1 = 0.0;

public:
    LinearModel13(double _a1, double _a2, double _b) 
        : a1(_a1), a2(_a2), b(_b) {}

    void reset() override {
        y_t = 0.0;
        y_t_minus_1 = 0.0;
    }

    double step(double u) override {
        double y_next = a1 * y_t + a2 * y_t_minus_1 + b * u;
        y_t_minus_1 = y_t;
        y_t = y_next;
        return y_t;
    }
};