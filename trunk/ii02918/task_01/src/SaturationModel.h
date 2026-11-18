#pragma once
#include "DynamicModel.h"
#include <sstream>
//  SaturationModel Ч реализаци€ Model 2.2
//  ћодель линейной системы с насыщением на входе
//  ‘ункци€ насыщени€ ограничивает вход снизу Umin и сверху Umax.
class SaturationModel : public DynamicModel
{
private:
    double a;
    double b;
    double uMin;    // нижн€€ граница насыщени€
    double uMax;    // верхн€€ граница насыщени€
    double y = 0.0;       // текущее состо€ние

    // ‘ункци€ насыщени€
    double saturate(double u) const
    {
        if (u > uMax) return uMax;
        if (u < uMin) return uMin;
        return u;
    }

public:
    SaturationModel(double a_, double b_, double uMin_, double uMax_)
        : a(a_), b(b_), uMin(uMin_), uMax(uMax_)
    {
    }

    double advance(double u) override
    {
        const double uSat = saturate(u);
        y = a * y + b * uSat;
        return y;
    }

    void reset() override
    {
        y = 0.0;
    }

    std::string describe() const override
    {
        std::ostringstream oss;
        oss << "Model 2.2 (Saturation, Umin=" << uMin
            << ", Umax=" << uMax << ")";
        return oss.str();
    }

    // Ќелинейна€ модель Ч линейный анализ устойчивости неприменим
    bool verifyStability() const override
    {
        return true;
    }

    std::string getWarningMessage() const override
    {
        return "";
    }
};