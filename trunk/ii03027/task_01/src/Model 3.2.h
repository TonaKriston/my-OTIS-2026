#pragma once
#include "Model.h"

class DiffEqModel3_2 : public Model {
private:
    double b = 0.0;
    double dt = 0.0;
    double y_t = 0.0;

public:
    DiffEqModel3_2(double _b, double _dt) : b(_b), dt(_dt) {}

    void reset() override { 
        y_t = 0.0; 
    }

    double step(double u) override {
        double dy_dt = b * u;
        y_t = y_t + dt * dy_dt;
        return y_t;
    }
};