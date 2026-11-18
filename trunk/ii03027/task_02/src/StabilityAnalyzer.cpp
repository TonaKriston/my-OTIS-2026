#include "StabilityAnalyzer.h"
#include <cmath>
#include <sstream>

bool StabilityAnalyzer::isStable(double a1, double a2) {
    double D = a1 * a1 + 4.0 * a2;
    if (D >= 0) {
        double z1 = (a1 + std::sqrt(D)) / 2.0;
        double z2 = (a1 - std::sqrt(D)) / 2.0;
        return (std::abs(z1) < 1.0) && (std::abs(z2) < 1.0);
    } else {
        return std::abs(a2) < 1.0;
    }
}

std::string StabilityAnalyzer::getRootsInfo(double a1, double a2) {
    double D = a1 * a1 + 4.0 * a2;
    std::stringstream ss;
    if (D >= 0) {
        ss << "z1 = " << (a1 + std::sqrt(D)) / 2.0 << ", z2 = " << (a1 - std::sqrt(D)) / 2.0;
    } else {
        ss << "Комплексные корни, |z| = " << std::sqrt(std::abs(a2));
    }
    return ss.str();
}