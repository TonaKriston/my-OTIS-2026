#pragma once

#include "AbstractSystem.h"
#include <sstream>
#include <cmath>

class LinearARXModel : public AbstractSystem
{
private:
    double a1;
    double a2;
    double b1;
    double b2;

    double yCurr = 0.0;
    double yPrev = 0.0;
    double uPrev = 0.0;

public:
    LinearARXModel(double a1_, double a2_, double b1_, double b2_);

    double step(double u) override;
    void reset() override;
    std::string getDescription() const override;
    bool checkStability() const override;
    std::string getWarningMessage() const override;
};