#pragma once

#include "Model.h"

class LineModel_1_10 : public Model
{
private:
    double a1;
    double b1;
    double b2;
    double y = 0.0;
    double u_prev1 = 0.0;
    double u_prev2 = 0.0;

public:
    LineModel_1_10(double a1_, double b1_, double b2_);
    void reset() override;
    double step(double u) override;
    const char *name() override;
};
