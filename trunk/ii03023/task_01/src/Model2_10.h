#ifndef MODEL2_10_H
#define MODEL2_10_H

#include "Model.h"

class Model2_10 : public Model {
private:
    double a;
    double b;

public:
    explicit Model2_10(double a_val, double b_val);
    
    double calculateNext(double u, int step) override;
};

#endif // MODEL2_10_H