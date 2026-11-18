#pragma once

/**
 * Анализ устойчивости линейной дискретной модели второго порядка:
 * y[k+1] = a1*y[k] + a2*y[k-1] + b*u[k].
 *
 * Характеристический многочлен: z^2 - a1*z - a2.
 * Для асимптотической устойчивости все корни должны лежать строго
 * внутри единичного круга |z| < 1.
 */
struct StabilityResult
{
    bool stable;
    double root1Real;
    double root1Imag;
    double root2Real;
    double root2Imag;
    const char* message;
};

StabilityResult analyzeModel1_3Stability(double a1, double a2);
