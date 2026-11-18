#include "Model1_8.h"

Model1_8::Model1_8(double a1_val, double a2_val, double b1_val, double b2_val)
    : Model(a1_val, "Model 1.8 (Generalized Autoregressive Linear Model)[cite: 1]"), 
      a1(a1_val), a2(a2_val), b1(b1_val), b2(b2_val) {}

double Model1_8::calculateStep(double u_current, double u_prev1_val, double u_prev2_val) {
    (void)u_prev2_val;
    double y_next = a1 * y_prev1 + a2 * y_prev2 + b1 * u_current + b2 * u_prev1_val;
    y_prev2 = y_prev1;
    y_prev1 = y_next;
    return y_next;
}

void Model1_8::reset() {
    y_prev1 = 0.0;
    y_prev2 = 0.0;
}