#pragma once

#include "AbstractSystem.h"

class CubicODE : public AbstractSystem
{
private:
    double a;
    double b;
    double dt;
    double yCurr = 1.0;

public:
    CubicODE(double a_, double b_, double dt_);

    double step(double u) override;
    void reset() override;
    std::string getDescription() const override;
    bool checkStability() const override;
    std::string getWarningMessage() const override;

    double getDt() const { return dt; }
};