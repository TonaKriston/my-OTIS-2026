#include "LinearModel13.h"

LinearModel13::LinearModel13(double _a1, double _a2, double _b) 
    : a1(_a1), a2(_a2), b(_b) {}

void LinearModel13::reset() {
    y_t = 0.0;
    y_t_minus_1 = 0.0;
}

double LinearModel13::step(double u) {
    double y_next = a1 * y_t + a2 * y_t_minus_1 + b * u;
    y_t_minus_1 = y_t;
    y_t = y_next;
    return y_t;
}