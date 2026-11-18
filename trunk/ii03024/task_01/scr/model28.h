#ifndef MODEL28_H
#define MODEL28_H

#include "model.h"

class Model28 : public Model {
private:
    double a, b, c;
    double y = 0.0;
    double y_prev = 0.0;

public:
    Model28(double a, double b, double c);
    double nextStep(double u, double dt) override;
    std::string getName() const override;
    void reset() override;
};

#endif