#ifndef MODEL36_H
#define MODEL36_H

#include "model.h"
#include <cmath>

class Model36 : public Model {
private:
    double a, b;
    double y = 0.0;

public:
    Model36(double a, double b);
    double nextStep(double u, double dt) override;
    std::string getName() const override;
    void reset() override;
};

#endif