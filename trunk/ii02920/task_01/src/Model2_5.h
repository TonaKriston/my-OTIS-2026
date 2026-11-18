#pragma once

#include "Model.h"
#include <cmath>

// Вариант 25. y(tau+1) = a*y(tau) + b*sign(u)*(1-exp(-abs(u))).
class Model2_5 : public Model {
private:
    double a;
    double b;
    double initialY;
    double y;

    static double sign(double u) {
        if (u > 0.0) {
            return 1.0;
        }
        if (u < 0.0) {
            return -1.0;
        }
        return 0.0;
    }

public:
    Model2_5(double a, double b, double y0 = 0.0)
        : a(a), b(b), initialY(y0), y(y0) {
    }

    double next(double u) override {
        // -expm1(-|u|) равно 1-exp(-|u|) и точнее для очень малого |u|.
        double nonlinearInput = sign(u) * (-std::expm1(-std::abs(u)));
        y = a * y + b * nonlinearInput;
        return y;
    }

    void reset() override {
        y = initialY;
    }

    double value() const override {
        return y;
    }

    std::string name() const override {
        return "Model 2.5";
    }

    bool isStable() const override {
        // Нелинейность относится ко входу. Разность двух состояний
        // при одинаковом управлении удовлетворяет e(tau+1) = a*e(tau).
        return std::abs(a) < 1.0;
    }

    std::string stabilityWarning() const override {
        if (isStable()) {
            return "";
        }
        if (std::abs(a) == 1.0) {
            return "[WARN] |a| = 1: initial deviations do not decay; response may accumulate.";
        }
        return "[WARN] |a| > 1: initial deviations can grow despite bounded input nonlinearity.";
    }
};
