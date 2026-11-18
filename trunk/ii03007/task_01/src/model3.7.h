#pragma once
#include <cmath>
#include "model.h"

class model3_7 : public BaseModel {
    private:
        double a;
        double b;
        double h; 
        double y = 0;

    public:
    model3_7(double a, double b, double h)
    : a(a), b(b), h(h)
    {
    }

    double stepForward(double u) override {
        double f = -std::exp(a) * y + b * u;
        y = y + h * f;
        return y;
    }

    void clearState() override {
        y = 0;
    }
};
