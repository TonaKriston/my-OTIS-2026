#pragma once

#include "AbstractSystem.h"

class SaturationModel : public AbstractSystem
{
private:
    double a;
    double b;
    double uMin;
    double uMax;

    double yCurr = 0.0;

    double saturate(double u) const;

public:
    SaturationModel(double a_, double b_, double uMin_, double uMax_);

    double step(double u) override;
    void reset() override;
    std::string getDescription() const override;
};