#include "InputGenerator.h"
#include <cmath>

InputGenerator::InputGenerator(int type, double constant_u)
    : type(type), constant_u(constant_u) {}

double InputGenerator::Next() {
    double u;
    switch (type) {
        case 0:  u = constant_u;                            break;
        case 1:  u = (t == 0) ? 1.0 : 0.0;                  break;
        default: u = std::sin(static_cast<double>(t));      break;
    }
    ++t;
    return u;
}

void InputGenerator::Reset() { t = 0; }