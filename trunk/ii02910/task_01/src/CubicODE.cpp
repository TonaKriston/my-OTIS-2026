#include "CubicODE.h"
#include <cmath>
#include <sstream>

CubicODE::CubicODE(double a_, double b_, double dt_)
    : a(a_), b(b_), dt(dt_)
{
}

double CubicODE::step(double u)
{
    double dydt = a * yCurr * yCurr * yCurr + b * u;
    yCurr = yCurr + dt * dydt;
    return yCurr;
}

void CubicODE::reset()
{
    yCurr = 1.0;
}

std::string CubicODE::getDescription() const
{
    return "Model 3.6 (Cubic Growth and Control)";
}

bool CubicODE::checkStability() const
{
    double yRef = 1.0;
    double deriv = 3.0 * a * yRef * yRef;
    double z = 1.0 + dt * deriv;
    return std::abs(z) < 1.0;
}

std::string CubicODE::getWarningMessage() const
{
    if (checkStability())
    {
        return "";
    }

    std::ostringstream oss;
    double yRef = 1.0;
    double deriv = 3.0 * a * yRef * yRef;
    double z = 1.0 + dt * deriv;
    oss << "[WARNING] Model 3.6: Euler step unstable at y = " << yRef
        << " (|1 + dt*3*a*y^2| = " << std::abs(z)
        << " >= 1). Decrease dt or |a|.";
    return oss.str();
}