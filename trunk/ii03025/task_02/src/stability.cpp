#include "stability.h"
#include <cmath>

// Модель 2.3: корень z = a, устойчиво при |a| < 1
bool isStable23(double a) {
    return std::abs(a) < 1.0;
}

// Модель 3.7: корень z = 1 - dt*e^a, устойчиво при |z| < 1
bool isStable37(double a, double dt) {
    double z = 1.0 - dt * std::exp(a);
    return std::abs(z) < 1.0;
}

// Модель 1.9: z^3 - a1*z^2 - a2*z - a3 = 0, критерий Джури
bool isStable19(double a1, double a2, double a3) {
    double c2 = -a1;
    double c1 = -a2;
    double c0 = -a3;

    double p1  = 1 + c2 + c1 + c0;     // P(1)
    double pm1 = -1 + c2 - c1 + c0;    // P(-1)

    if (!(p1 > 0)) return false;
    if (!(-pm1 > 0)) return false;
    if (!(std::abs(c0) < 1)) return false;

    double b0 = c0 * c0 - 1;
    double b2 = c0 * c2 - c1;
    return std::abs(b0) > std::abs(b2);
}
