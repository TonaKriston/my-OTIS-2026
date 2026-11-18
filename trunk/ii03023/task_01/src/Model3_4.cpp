#include "Model3_4.h"

Model3_4::Model3_4(double a_val, double dt_val) 
    : Model(0.0), a(a_val), dt(dt_val) {}

double Model3_4::calculateNext(double u, int step) {
    double y_prev1 = getPrev1(); // Используем геттер вместо прямого доступа к private полю
    
    double derivative = -a * (y_prev1 * y_prev1);
    double next_y = y_prev1 + dt * derivative + u;
    
    setPrev(next_y, 0.0, 0.0);
    return next_y;
}