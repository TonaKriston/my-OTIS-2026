#pragma once

#include "Model.h"

class Model3_9 final : public Model
{
private:
    double b;

public:
    explicit Model3_9(double b);

    double calculateNext(
        double y,
        double yPrev,
        double yPrev2,
        double u,
        double dt
    ) const override;

    const char* getName() const override;
};
