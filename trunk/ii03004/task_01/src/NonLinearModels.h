#pragma once

#include "Model.h"

// Model 2.6 (Asymmetric Polynomial Backlash Simulation).
// Формула (см. formula()): следующее значение y равно a1 * y(tau) в кубе,
// минус a2 * y(tau-1), плюс b * u(tau) в квадрате.
class Model2_6 final : public Model
{
public:
    Model2_6(double a1, double a2, double b);

    std::string name() const override;
    std::string formula() const override;

protected:
    double calculateNext(double u) const override;

private:
    double a1_;
    double a2_;
    double b_;
};
