#include "Model2_6.h"

Model2_6::Model2_6(double a1, double a2, double b)
    : a1(a1), a2(a2), b(b)
{
}

double Model2_6::calculateNext(
    double y,
    double yPrev,
    double,
    double u,
    double
) const
{
    return a1 * y * y * y - a2 * yPrev + b * u * u;
}

const char* Model2_6::getName() const
{
    return "Model 2.6 - Asymmetric Polynomial Backlash Simulation";
}
