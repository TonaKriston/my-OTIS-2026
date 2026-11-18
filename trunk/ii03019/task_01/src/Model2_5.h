#ifndef MODEL2_5_H
#define MODEL2_5_H

#include "Model.h"

class Model2_5 : public Model {
private:
    double a;
    double b;
    static double sign(double val);

public:
    explicit Model2_5(double a_val, double b_val, double initial_y = 0.0);
    double step(double u) override;
};

#endif