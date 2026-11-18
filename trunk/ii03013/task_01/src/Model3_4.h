#pragma once

#include "models.h"

class Model3_4 : public Model {
private:
    double a;

public:
    explicit Model3_4(double a);

    double next(
        double y,
        double u,
        double delayedU,
        double dt
    ) const override;

    std::string name() const override;
};