#include "Model3_2.h"

Model3_2::Model3_2(double b_val)
    : Model(0.0, "Model 3.2 (Pure Constant Input Drive)[cite: 3]"), b_coef(b_val) {}

double Model3_2::calculateStep(double u_current, double u_prev1, double u_prev2) {
    (void)u_prev1;
    (void)u_prev2;
    double y_next = y_prev + b_coef * u_current;
    y_prev = y_next;
    return y_next;
}

void Model3_2::reset() {
    y_prev = 0.0;
}