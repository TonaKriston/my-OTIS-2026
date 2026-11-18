#ifndef MODEL3_4_H
#define MODEL3_4_H

#include "Model.h"

class Model3_4 : public Model {
private:
    double a;
    double dt;

public:
    explicit Model3_4(double a_val, double dt_val);
    
    double calculateNext(double u, int step) override;
};

#endif // MODEL3_4_H