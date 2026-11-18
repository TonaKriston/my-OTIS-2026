#include "Simple_Differential_Equation.h"
#include <cmath>

Simple_Differential_Equation::Simple_Differential_Equation(
        double b, double dt, int type, double u_const)
    : b(b), dt(dt), input(type, u_const) {}

double Simple_Differential_Equation::NextStep() {
    last_u = input.Next();
    y += dt * b * std::sin(last_u);
    return y;
}

double Simple_Differential_Equation::GetY()     const { return y; }
double Simple_Differential_Equation::GetLastU() const { return last_u; }