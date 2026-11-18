#ifndef MODEL1_6_H
#define MODEL1_6_H

#include "Model.h"

class Model1_6 : public Model {
private:
    double a1;
    double a2;
    double a3;
    double b;

public:
    explicit Model1_6(double a1_val, double a2_val, double a3_val, double b_val);
    
    double calculateNext(double u, int step) override;
};

#endif // MODEL1_6_H