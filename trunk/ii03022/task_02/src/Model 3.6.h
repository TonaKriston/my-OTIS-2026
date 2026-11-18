#pragma once
#include "../../task_01/src/Model.h"

class CubicGrowthAndControlModel : public Model{
    private:
        double a;
        double b;
        double h;
        double y;
        double y0;

    public:
    CubicGrowthAndControlModel(double a, double b, double h, double y0 = 0)
    : a(a), b(b), h(h), y(y0), y0(y0)
    {
    }

    double nextStep(double u) override{
        double f = a * y * y * y + b * u;
        y = y + h * f;
        return y;
    }

    void reset() override{
        y = y0;
    }
};