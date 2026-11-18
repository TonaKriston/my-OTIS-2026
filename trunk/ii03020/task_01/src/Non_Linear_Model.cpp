#include "Non_Linear_Model.h"
#include <cmath>

Non_linear_Model::Non_linear_Model(double a, double b, double c, double d,
                                   int type, double u_const)
    : a(a), b(b), c(c), d(d),
      input(type, u_const) {}

double Non_linear_Model::NextStep() {
    u_t1 = u_t;
    u_t  = input.Next();
    last_u = u_t;

    double y_new = a * y - b * y_prev * y_prev
                 + c * u_t + d * std::sin(u_t1);
    y_prev = y;
    y      = y_new;
    return y;
}

double Non_linear_Model::GetY()     const { return y; }
double Non_linear_Model::GetLastU() const { return last_u; }