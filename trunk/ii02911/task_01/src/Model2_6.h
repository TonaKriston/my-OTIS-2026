#pragma once

#include "Model.h"

class Model2_6 final : public Model
{
private:
    double a1;
    double a2;
    double b;

public:
    Model2_6(double a1, double a2, double b);

    double calculateNext(
        double y,
        double yPrev,
        double yPrev2,
        double u,
        double dt
    ) const override;

    const char* getName() const override;
};
