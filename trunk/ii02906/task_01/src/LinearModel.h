#pragma once
#include "Model.h"

class LinearModel : public Model {
private:
    double a1;
    double a2;
    double a3;
    double b;
    double y_t = 0;
    double y_t1 = 0;
    double y_t2 = 0;
public:
    LinearModel(double _a1, double _a2, double _a3, double _b);
    double step(double u) override;
    void reset() override;
    bool isStable() const;
};
