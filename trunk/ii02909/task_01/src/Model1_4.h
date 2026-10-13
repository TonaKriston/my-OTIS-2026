#pragma once

#include "Model.h"
#include <cmath>
#include <sstream>

// Model 1.4: Discrete Integrator with Loss
// y_{t+1} = a * y_t + b * (u_t - u_{t-1})
class Model1_4 : public Model {
private:
    // Границы устойчивости: |a| < STABILITY_BOUND
    static constexpr double STABILITY_BOUND = 1.0;

    double a;
    double b;
    double y = 0.0;        // текущее y_t
    double u_prev = 0.0;   // предыдущее u_{t-1}

public:
    Model1_4(double a_, double b_)
        : a(a_), b(b_) {
    }

    double next(double u) override {
        const double y_next = a * y + b * (u - u_prev);
        u_prev = u;
        y = y_next;
        return y;
    }

    void reset() override {
        y = 0.0;
        u_prev = 0.0;
    }

    std::string name() const override {
        return "Model 1.4 (Discrete Integrator with Loss)";
    }

    // Устойчивость: характеристическое уравнение z - a = 0, корень z = a
    // Условие: |a| < 1
    bool isStable() const override {
        return std::abs(a) < STABILITY_BOUND;
    }

    std::string stabilityWarning() const override {
        const double abs_a = std::abs(a);
        if (abs_a < STABILITY_BOUND) {
            return "";
        }

        std::ostringstream oss;
        if (abs_a == STABILITY_BOUND) {
            // На границе устойчивости: незатухающие колебания
            oss << "[WARN] Model 1.4: |a| = 1 (" << a
                << "), system is on the stability boundary (undamped oscillations).";
        }
        else {
            // |a| > 1 — система расходится
            oss << "[WARN] Model 1.4: |a| = " << abs_a
                << " >= 1, system is UNSTABLE, y will diverge!";
        }
        return oss.str();
    }
};