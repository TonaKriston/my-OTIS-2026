#pragma once
#include "../../task_01/src/model.h"

class model3_7 : public BaseModel {
    private:
        double a;
        double b;
        double h;
        double y;
        double y0;

    public:
    model3_7(double a, double b, double h, double y0 = 0.0)
    : a(a), b(b), h(h), y(y0), y0(y0)
    {
    }

    double stepForward(double u) override {
        double f = -std::exp(a) * y + b * u;
        y = y + h * f;
        return y;
    }

    void clearState() override {
        y = y0;
    }
};