#include "linear_model_1_9.h"

LinearModel1_9::LinearModel1_9(double a1, double a2, double a3, double b1)
    : a1_(a1), a2_(a2), a3_(a3), b1_(b1) {}

double LinearModel1_9::step(double u) {
    double y_next = a1_ * y_ + a2_ * y_prev1_ + a3_ * y_prev2_ + b1_ * u;
    
    y_prev2_ = y_prev1_;
    y_prev1_ = y_;
    y_ = y_next;
    
    return y_;
}

void LinearModel1_9::reset() {
    y_ = 0.0;
    y_prev1_ = 0.0;
    y_prev2_ = 0.0;
}