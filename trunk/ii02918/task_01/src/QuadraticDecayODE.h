#pragma once
#include "DynamicModel.h"
#include <cmath>
#include <sstream>
//  QuadraticDecayODE — реализация Model 3.8
//  Модель с линейно-квадратичным затуханием
//  Решается численно методом Эйлера с шагом dt.
class QuadraticDecayODE : public DynamicModel
{
private:
    static constexpr double STABILITY_LIMIT = 1.0;
    static constexpr double INITIAL_Y = 1.0;   // y(0) = 1

    double a1;
    double a2;
    double b;
    double dt;      // шаг интегрирования
    double y = INITIAL_Y;       // текущее состояние

public:
    QuadraticDecayODE(double a1_, double a2_, double b_, double dt_)
        : a1(a1_), a2(a2_), b(b_), dt(dt_)
    {
    }

    double advance(double u) override
    {
        // Производная по формуле линейно-квадратичного затухания
        const double dydt = -a1 * y - a2 * y * y + b * u;

        // Шаг интегрирования методом Эйлера
        y = y + dt * dydt;
        return y;
    }

    void reset() override
    {
        y = INITIAL_Y;
    }

    std::string describe() const override
    {
        std::ostringstream oss;
        oss << "Model 3.8 (Quadratic Decay ODE, dt=" << dt << ")";
        return oss.str();
    }

    // Проверка устойчивости численной схемы Эйлера
    bool verifyStability() const override
    {
        // Оценка устойчивости численной схемы по начальному значению
        const double multiplier = 1.0 + dt * (-a1 - 2.0 * a2 * INITIAL_Y);
        return std::abs(multiplier) < STABILITY_LIMIT;
    }

    std::string getWarningMessage() const override
    {
        const double multiplier = 1.0 + dt * (-a1 - 2.0 * a2 * INITIAL_Y);
        if (std::abs(multiplier) < STABILITY_LIMIT) return "";

        std::ostringstream oss;
        oss << "[WARNING] Model 3.8: Euler scheme may be unstable. "
            << "|1 + dt*(-a1 - 2*a2*y0)| = " << std::abs(multiplier)
            << " >= 1. Decrease dt.";
        return oss.str();
    }
};