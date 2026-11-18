#pragma once
#include "Model.h"

class NonlinearModel2_10 : public Model {
private:
    double a;
    double b;
    double y_t = 0.0;
public:
    NonlinearModel2_10(double _a, double _b);
    void reset() override;
    double step(double u) override;
};