#pragma once

#include "models.h"

class Model2_2 : public Model {
private:
    double a;
    double b;
    double uMin;
    double uMax;

public:
    Model2_2(double a, double b, double uMin, double uMax);

    double next(
        double y,
        double u,
        double delayedU,
        double dt
    ) const override;

    std::string name() const override;
};