#ifndef MODEL3_3_H
#define MODEL3_3_H

#include "Model.h"

class Model3_3 : public Model {
private:
    double a;
    double b;
    double dt;

public:
    explicit Model3_3(double a_val, double b_val, double dt_val = 0.1, double initial_y = 0.0);
    double step(double u) override;
};

#endif