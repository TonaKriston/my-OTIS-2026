#pragma once

#include "Model.h"

// Model 3.8 (Combined Linear-Quadratic Decay).
// Дифференциальное уравнение (см. formula()) решается методом Эйлера с шагом dt:
// новое значение y получается из текущего прибавлением dt, умноженного
// на правую часть уравнения при текущих y и u.
class Model3_8 final : public Model
{
public:
    Model3_8(double a1, double a2, double b, double dt);

    std::string name() const override;
    std::string formula() const override;

protected:
    double calculateNext(double u) const override;

private:
    double a1_;
    double a2_;
    double b_;
    double dt_;
};
