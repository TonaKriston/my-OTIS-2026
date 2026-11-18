#pragma once
#include "Model.h"

class Model27 final : public Model {
public:
    Model27(double a, double b, double epsilon);
    std::string name() const override;
    double calculateNext(double u, int step, const std::vector<double>& history,
                         double dt) const override;
private:
    double a_;
    double b_;
    double epsilon_;
};
