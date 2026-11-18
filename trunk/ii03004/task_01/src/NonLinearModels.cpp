#include "NonLinearModels.h"

Model2_6::Model2_6(double a1, double a2, double b)
    : a1_(a1), a2_(a2), b_(b)
{
}

std::string Model2_6::name() const
{
    return "Model 2.6 (Asymmetric Polynomial Backlash Simulation)";
}

std::string Model2_6::formula() const
{
    return "y(t+1) = a1*y(t)^3 - a2*y(t-1) + b*u(t)^2";
}

double Model2_6::calculateNext(double u) const
{
    const double y = currentY();
    return a1_ * y * y * y - a2_ * previousY() + b_ * u * u;
}
