#pragma once

#include "Model.h"
#include <cmath>

// Вариант 25. y(tau+1) = a1*y(tau) + b1*u(tau) + b2*u(tau-2).
class Model1_10 : public Model {
private:
    double a1;
    double b1;
    double b2;
    double initialY;
    double y;
    double previousU = 0.0;
    double delayedU = 0.0;

public:
    Model1_10(double a1, double b1, double b2, double y0 = 0.0)
        : a1(a1), b1(b1), b2(b2), initialY(y0), y(y0) {
    }

    double next(double u) override {
        double nextY = a1 * y + b1 * u + b2 * delayedU;
        // До сдвига delayedU хранит u(tau-2), previousU хранит u(tau-1).
        delayedU = previousU;
        previousU = u;
        y = nextY;
        return y;
    }

    void reset() override {
        y = initialY;
        previousU = 0.0;
        delayedU = 0.0;
    }

    double value() const override {
        return y;
    }

    std::string name() const override {
        return "Model 1.10";
    }

    bool isStable() const override {
        return std::abs(a1) < 1.0;
    }

    std::string stabilityWarning() const override {
        if (isStable()) {
            return "";
        }
        if (std::abs(a1) == 1.0) {
            return "[WARN] |a1| = 1: stability boundary; initial deviations do not decay.";
        }
        return "[WARN] |a1| > 1: UNSTABLE; non-zero initial deviations can grow.";
    }
};
