#include "StabilityAnalyzer.h"

#include <cmath>
#include <complex>
#include <limits>

StabilityResult analyzeModel1_3Stability(double a1, double a2)
{
    const double nan = std::numeric_limits<double>::quiet_NaN();
    if (!std::isfinite(a1) || !std::isfinite(a2))
        return {false, nan, nan, nan, nan, "Коэффициенты должны быть конечными числами."};

    // Корни характеристического многочлена второй степени от z с коэффициентами 1, -a1, -a2.
    const std::complex<double> discriminant(a1 * a1 + 4.0 * a2, 0.0);
    const std::complex<double> sqrtDiscriminant = std::sqrt(discriminant);
    const std::complex<double> root1 = (a1 + sqrtDiscriminant) / 2.0;
    const std::complex<double> root2 = (a1 - sqrtDiscriminant) / 2.0;

    const bool stable = std::abs(root1) < 1.0 && std::abs(root2) < 1.0;
    return {
        stable,
        root1.real(), root1.imag(),
        root2.real(), root2.imag(),
        stable ? "Модель асимптотически устойчива."
               : "Предупреждение: модель не является асимптотически устойчивой (есть корень |z| >= 1)."
    };
}
