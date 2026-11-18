#include "SaturationModel.h"

SaturationModel::SaturationModel(double a_, double b_, double uMin_, double uMax_)
    : a(a_), b(b_), uMin(uMin_), uMax(uMax_)
{
}

double SaturationModel::saturate(double u) const
{
    if (u > uMax) return uMax;
    if (u < uMin) return uMin;
    return u;
}

double SaturationModel::step(double u)
{
    yCurr = a * yCurr + b * saturate(u);
    return yCurr;
}

void SaturationModel::reset()
{
    yCurr = 0.0;
}

std::string SaturationModel::getDescription() const
{
    return "Model 2.2 (Actuator Saturation Non-linearity)";
}