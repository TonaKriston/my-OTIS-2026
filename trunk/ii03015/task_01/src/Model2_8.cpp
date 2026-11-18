#include "Model2_8.h"
#include <cmath>

Model2_8::Model2_8(double a, double b, double c): a(a), b(b), c(c) {}

double Model2_8::calculateNext(double y, double yPrev, double /*yPrev2*/, double u, double /*dt*/) const
{
    return a * y * (1.0 - y) + b * u + c * std::sin(yPrev * u);
}

const char* Model2_8::getName() const
{
    return "Model 2.8 - Chaotic Logistic Map Disturbance";
}
