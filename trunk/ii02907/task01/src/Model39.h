#pragma once
#include "Model.h"

class Model39 final : public Model {
public:
    explicit Model39(double value);
    std::string name() const override;
    double calculateNext(double u, int step, const std::vector<double>& history,
                         double dt) const override;
private:
    double b_;
};
