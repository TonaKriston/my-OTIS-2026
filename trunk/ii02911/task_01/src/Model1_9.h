#pragma once

#include "Model.h"

class Model1_9 final : public Model
{
private:
    double a1;
    double a2;
    double a3;
    double b1;

public:
    Model1_9(double a1, double a2, double a3, double b1);

    double calculateNext(
        double y,
        double yPrev,
        double yPrev2,
        double u,
        double dt
    ) const override;

    const char* getName() const override;
};
