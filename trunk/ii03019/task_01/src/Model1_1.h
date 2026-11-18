#ifndef MODEL1_1_H
#define MODEL1_1_H

#include "Model.h"

class Model1_1 : public Model {
private:
    double a;
    double b;

public:
    explicit Model1_1(double a_val, double b_val, double initial_y = 0.0);
    double step(double u) override;
};

#endif