#pragma once

#include "Model.h"
#include <cmath>

// Вариант 25. dy/dt = -a*y + b + u.
// Явный метод Эйлера: y(tau+1) = y(tau) + dt*(-a*y(tau) + b + u(tau)).
class Model3_10 : public Model {
private:
    double a;
    double b;
    double dt;
    double initialY;
    double y;

public:
    Model3_10(double a, double b, double dt, double y0 = 0.0)
        : a(a), b(b), dt(dt), initialY(y0), y(y0) {
    }

    double next(double u) override {
        y = y + dt * (-a * y + b + u);
        return y;
    }

    void reset() override {
        y = initialY;
    }

    double value() const override {
        return y;
    }

    std::string name() const override {
        return "Model 3.10";
    }

    bool isStable() const override {
        return std::abs(1.0 - a * dt) < 1.0;
    }

    std::string stabilityWarning() const override {
        if (isStable()) {
            return "";
        }
        return "[WARN] |1-a*dt| >= 1: Euler scheme does not damp initial deviations. Check a and dt.";
    }
};
