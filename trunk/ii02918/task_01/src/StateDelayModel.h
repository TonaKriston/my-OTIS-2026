#pragma once
#include "DynamicModel.h"
#include <deque>
#include <cmath>
#include <sstream>

//  StateDelayModel — реализация Model 1.5
//  Модель с задержкой по состоянию на k шагов

class StateDelayModel : public DynamicModel
{
private:
    // Порог устойчивости: |z| < STABILITY_LIMIT
    static constexpr double STABILITY_LIMIT = 1.0;

    double a1;
    double a2;
    double b;
    int    k;                     

    std::deque<double> history; 

public:
    StateDelayModel(double a1_, double a2_, double b_, int k_)
        : a1(a1_), a2(a2_), b(b_), k(k_)
    {
        for (int i = 0; i <= k; ++i)
        {
            history.push_back(0.0);
        }
    }

    double advance(double u) override
    {
        // Значение y из истории на k шагов назад
        const double yDelayed = history.front();

        // текущее y_t — последнее в истории
        const double yCurrent = history.back();

        // Формула: задержанное состояние домножается на a2
        const double yNext = a1 * yCurrent + a2 * yDelayed + b * u;

        // Сдвигаем историю: удаляем старое, добавляем новое
        history.pop_front();
        history.push_back(yNext);

        return yNext;
    }

    void reset() override
    {
        // Начальные значения y = 0, храним k+1 элементов
        history.clear();
        for (int i = 0; i <= k; ++i)
        {
            history.push_back(0.0);
        }
    }

    std::string describe() const override
    {
        std::ostringstream oss;
        oss << "Model 1.5 (State Delay, k=" << k << ")";
        return oss.str();
    }

    bool verifyStability() const override
    {
        // Упрощённый критерий устойчивости: |a1| + |a2| < 1
        return (std::abs(a1) + std::abs(a2)) < STABILITY_LIMIT;
    }

    std::string getWarningMessage() const override
    {
        const double sum = std::abs(a1) + std::abs(a2);
        if (sum < STABILITY_LIMIT) return "";

        std::ostringstream oss;
        oss << "[WARNING] Model 1.5: |a1|+|a2| = " << sum
            << " >= 1. Possible instability.";
        return oss.str();
    }
};