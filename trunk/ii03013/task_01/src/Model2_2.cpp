#include "Model2_2.h"

Model2_2::Model2_2(double a, double b, double uMin, double uMax)
    : a(a), b(b), uMin(uMin), uMax(uMax) {
}

double Model2_2::next(
    double y,
    double u,
    double delayedU,
    double dt
) const {
    (void)delayedU;
    (void)dt;

    double saturatedU = u;

    if (saturatedU < uMin) {
        saturatedU = uMin;
    }

    if (saturatedU > uMax) {
        saturatedU = uMax;
    }

    return a * y + b * saturatedU;
}

std::string Model2_2::name() const {
    return "Nonlinear Model 2.2";
}