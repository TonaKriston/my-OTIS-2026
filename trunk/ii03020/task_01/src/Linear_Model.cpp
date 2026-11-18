#include "Linear_Model.h"

Linear_Model::Linear_Model(double a, double b1, double b2, double b3,
                           int type, double u_const)
    : a(a), b1(b1), b2(b2), b3(b3),
      input(type, u_const) {}

double Linear_Model::NextStep() {
    u_t2 = u_t1;
    u_t1 = u_t;
    u_t  = input.Next();
    last_u = u_t;
    y = a * y + b1 * u_t + b2 * u_t1 + b3 * u_t2;
    return y;
}

double Linear_Model::GetY()     const { return y; }
double Linear_Model::GetLastU() const { return last_u; }