#include "Model.h"
#include <cmath>

Model::Model(double a, double b, double h) : a(a), b(b), h(h)
{
}

Model11::Model11(double a, double b) : Model(a, b, 0)
{
}

double Model11::calculate(double y, double u)
{
    return getA() * y + getB() * u;
}

Model25::Model25(double a, double b) : Model(a, b, 0)
{
}

double Model25::calculate(double y, double u)
{
    double sign = 0;
    if (u > 0)
        sign = 1;
    if (u < 0)
        sign = -1;

    return getA() * y + getB() * sign * (1 - std::exp(-std::abs(u)));
}

Model39::Model39(double b, double h) : Model(0, b, h)
{
}

double Model39::calculate(double y, double u)
{
    return y + getH() * getB() * std::tanh(u);
}