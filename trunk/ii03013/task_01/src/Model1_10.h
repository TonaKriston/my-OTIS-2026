#pragma once

#include "models.h"

class Model1_10 : public Model {
private:
    double a1;
    double b1;
    double b2;

public:
    Model1_10(double a1, double b1, double b2);

    double next(
        double y,
        double u,
        double delayedU,
        double dt
    ) const override;

    std::string name() const override;

    bool isStable() const override;

    std::string stabilityWarning() const override;
};