#include "LinearModels.h"

#include <cmath>

Model1_4::Model1_4(double a, double b)
    : a_(a), b_(b)
{
}

std::string Model1_4::name() const
{
    return "Model 1.4 (Discrete Integrator with Loss)";
}

std::string Model1_4::formula() const
{
    return "y(t+1) = a*y(t) + b*(u(t) - u(t-1))";
}

bool Model1_4::isStable() const
{
    return std::fabs(a_) < 1.0;
}

double Model1_4::calculateNext(double u) const
{
    return a_ * currentY() + b_ * (u - previousU());
}
