#include "Model1_6.h"
#include <cmath>

Model1_6::Model1_6(double a1_val, double a2_val, double a3_val, double b_val) 
    : Model(0.0), a1(a1_val), a2(a2_val), a3(a3_val), b(b_val) {}

double Model1_6::calculateNext(double u, int step) {
    double y_prev1 = getPrev1();
    double y_prev2 = getPrev2();
    double y_prev3 = getPrev3();

    double next_y = a1 * y_prev1 + a2 * y_prev2 + a3 * y_prev3 + b * u;
    
    setPrev(next_y, y_prev1, y_prev2);
    return next_y;
}