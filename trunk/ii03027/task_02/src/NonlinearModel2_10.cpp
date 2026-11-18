#include "NonlinearModel2_10.h"
#include <cmath>

NonlinearModel2_10::NonlinearModel2_10(double _a, double _b) : a(_a), b(_b) {}

void NonlinearModel2_10::reset() { 
    y_t = 0.0; 
}

double NonlinearModel2_10::step(double u) {
    y_t = a * std::tanh(y_t) + b * std::pow(u, 3);
    return y_t;
}