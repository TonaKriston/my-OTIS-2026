#pragma once
#include "Model.h"

class Model15 final : public Model {
public:
    Model15(double a1, double a2, double b, int delay);
    std::string name() const override;
    double calculateNext(double u, int step, const std::vector<double>& history,
                         double dt) const override;
private:
    double a1_;
    double a2_;
    double b_;
    int delay_;
};
