#pragma once

#include "Model.h"

class NonlinearModel_2_5 : public Model {
private:
    double a;
    double b;
    double y = 0.0;

public:
    NonlinearModel_2_5(double a_, double b_);
    void reset() override;
    double step(double u) override;
    const char* name() override;
};
