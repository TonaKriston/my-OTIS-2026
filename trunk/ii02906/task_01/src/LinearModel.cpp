#include "LinearModel.h"
#include <cmath>

LinearModel::LinearModel(double _a1, double _a2, double _a3, double _b)
    : a1(_a1), a2(_a2), a3(_a3), b(_b) {
}

double LinearModel::step(double u) {
    double y_next = a1 * y_t + a2 * y_t1 + a3 * y_t2 + b * u;
    y_t2 = y_t1;
    y_t1 = y_t;
    y_t = y_next;
    return y_next;
}

void LinearModel::reset() {
    y_t = 0; y_t1 = 0; y_t2 = 0;
}

bool LinearModel::isStable() const {
    if (std::abs(a3) >= 1) return false;
    if (1 + a1 + a2 + a3 <= 0) return false;
    if (1 - a1 + a2 - a3 <= 0) return false;
    if (std::abs(a3 * a3 - 1) <= std::abs(a3 * a1 - a2)) return false;
    return true;
}