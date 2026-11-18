#include "LinearARXModel.h"

LinearARXModel::LinearARXModel(double a1_, double a2_, double b1_, double b2_)
    : a1(a1_), a2(a2_), b1(b1_), b2(b2_)
{
}

double LinearARXModel::step(double u)
{
    double yNext = a1 * yCurr + a2 * yPrev + b1 * u + b2 * uPrev;
    yPrev = yCurr;
    yCurr = yNext;
    uPrev = u;
    return yCurr;
}

void LinearARXModel::reset()
{
    yCurr = 0.0;
    yPrev = 0.0;
    uPrev = 0.0;
}

std::string LinearARXModel::getDescription() const
{
    return "Model 1.8 (Generalized Autoregressive Linear Model)";
}

bool LinearARXModel::checkStability() const
{
    const double discriminant = a1 * a1 + 4.0 * a2;
    if (discriminant < 0.0)
    {
        return (a2 > -1.0) && (a2 < 1.0);
    }
    const double sqrtD = std::sqrt(discriminant);
    const double z1 = (a1 + sqrtD) / 2.0;
    const double z2 = (a1 - sqrtD) / 2.0;
    return (std::abs(z1) < 1.0) && (std::abs(z2) < 1.0);
}

std::string LinearARXModel::getWarningMessage() const
{
    if (checkStability())
    {
        return "";
    }

    std::ostringstream oss;
    if (const double discriminant = a1 * a1 + 4.0 * a2; discriminant < 0.0)
    {
        oss << "[WARNING] Model 1.8: complex roots, |a2| >= 1. System may be unstable.";
    }
    else
    {
        double sqrtD = std::sqrt(discriminant);
        double z1 = (a1 + sqrtD) / 2.0;
        double z2 = (a1 - sqrtD) / 2.0;
        oss << "[WARNING] Model 1.8: roots z1 = " << z1
            << ", z2 = " << z2 << " - outside unit circle. System is UNSTABLE.";
    }
    return oss.str();
}