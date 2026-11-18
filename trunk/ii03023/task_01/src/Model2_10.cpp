#include "Model2_10.h"
#include <cmath>

Model2_10::Model2_10(double a_val, double b_val) 
    : Model(0.0), a(a_val), b(b_val) {}

double Model2_10::calculateNext(double u, int step) {
    double y_prev1 = getPrev1();
    
    double next_y = a * y_prev1 + b * std::tanh(u);
    
    setPrev(next_y, 0.0, 0.0);
    return next_y;
}