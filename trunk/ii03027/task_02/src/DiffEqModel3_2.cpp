#include "DiffEqModel3_2.h"

DiffEqModel3_2::DiffEqModel3_2(double _b, double _dt) : b(_b), dt(_dt) {}

void DiffEqModel3_2::reset() { 
    y_t = 0.0; 
}

double DiffEqModel3_2::step(double u) {
    double dy_dt = b * u;
    y_t = y_t + dt * dy_dt;
    return y_t;
}