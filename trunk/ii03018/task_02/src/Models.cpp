#include "Models.h"
#include <cmath>
#include <iostream>

double getInput(int type, int tau) {
    if (type == 0) {
        return 1.0;
    }
    if (type == 1) {
        return (tau == 0) ? 1.0 : 0.0;
    }
    if (type == 2) {
        return std::sin(static_cast<double>(tau));
    }
    return 0.0;
}

StandardSignalGenerator::StandardSignalGenerator(int type)
    : signalType(type) {}

double StandardSignalGenerator::generate(int tau) const {
    return getInput(signalType, tau);
}

Model1_7::Model1_7(double a_val, double b1_val, double b2_val, double b3_val)
    : a(a_val), b1(b1_val), b2(b2_val), b3(b3_val) {
    checkStability(a);
}

bool Model1_7::checkStability(double a_coeff) {
    if (std::abs(a_coeff) >= 1.0) {
        std::cout << "[WARNING] System Model 1.7 is unstable: |a| = "
            << std::abs(a_coeff) << " >= 1.0 (pole outside unit circle)!\n";
        return false;
    }
    return true;
}

bool Model1_7::isStable() const {
    return checkStability(a);
}

double Model1_7::nextStep(double u) {
    double y_next = a * y + b1 * u + b2 * u_prev1 + b3 * u_prev2;
    u_prev2 = u_prev1;
    u_prev1 = u;
    y = y_next;
    return y;
}

double Model1_7::stepWithGenerator(const ISignalGenerator& generator, int tau) {
    double u = generator.generate(tau);
    return nextStep(u);
}

void Model1_7::reset() {
    y = 0.0;
    u_prev1 = 0.0;
    u_prev2 = 0.0;
}

Model2_9::Model2_9(double a_val, double b_val)
    : a(a_val), b(b_val) {}

double Model2_9::nextStep(double u) {
    double sign_u = 0.0;
    if (u > 0.0) {
        sign_u = 1.0;
    }
    else if (u < 0.0) {
        sign_u = -1.0;
    }

    y = a * y + b * std::sqrt(std::abs(u)) * sign_u;
    return y;
}

void Model2_9::reset() {
    y = 0.0;
}

Model3_1::Model3_1(double a_val, double dt_val)
    : a(a_val), dt(dt_val) {}

double Model3_1::nextStep(double /*u*/) {
    y = y - dt * (a * y);
    return y;
}

void Model3_1::reset() {
    y = 1.0;
}