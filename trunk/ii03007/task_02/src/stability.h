#pragma once
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>

// Анализ устойчивости модели 1.6.
// Характеристическое уравнение: z^3 - a1*z^2 - a2*z - a3 = 0.
// Устойчиво, если все корни строго внутри единичного круга (|z| < 1).

struct CubicRoots {
    std::array<double, 3> re;   // действительные части корней
    std::array<double, 3> im;   // мнимые части (0 для вещественных корней)
    std::array<double, 3> mod;  // модули |z|
    bool stable;                // все |z| < 1
};

// Решение кубического уравнения (Кардано / тригонометрический метод, в double).
inline CubicRoots cubicRoots(double a1, double a2, double a3) {
    CubicRoots r{};
    const double PI = 3.14159265358979323846;

    // Уравнение вида z^3 + p*z^2 + q*z + rr = 0
    double p  = -a1;
    double q  = -a2;
    double rr = -a3;

    // Приведённый кубик t^3 + A*t + B = 0, замена z = t - p/3
    double A = q - p * p / 3.0;

    if (double B = 2.0 * p * p * p / 27.0 - p * q / 3.0 + rr; B * B / 4.0 + A * A * A / 27.0 > 0.0) {
        // Один вещественный корень + комплексно-сопряжённая пара
        double s  = std::sqrt(B * B / 4.0 + A * A * A / 27.0);
        double t1 = std::cbrt(-B / 2.0 + s) + std::cbrt(-B / 2.0 - s);
        r.re[0] = t1 - p / 3.0;
        r.im[0] = 0.0;
        double dq = 3.0 * t1 * t1 + 4.0 * A;   // положительно при B * B / 4.0 + A * A * A / 27.0 > 0
        r.re[1] = -t1 / 2.0 - p / 3.0;
        r.im[1] = 0.5 * std::sqrt(dq > 0.0 ? dq : 0.0);
        r.re[2] = r.re[1];
        r.im[2] = -r.im[1];
    } else {
        // Три вещественных корня (B * B / 4.0 + A * A * A / 27.0 = 0 - кратные)
        if (A >= 0.0) {
            // a1 = a2 = a3 = 0: тройной корень t = 0
            r.re[0] = -p / 3.0;
            r.re[1] = -p / 3.0;
            r.re[2] = -p / 3.0;
            r.im[0] = 0.0;
            r.im[1] = 0.0;
            r.im[2] = 0.0;
        } else {
            double m   = 2.0 * std::sqrt(-A / 3.0);
            double arg = 3.0 * B / (2.0 * A) * std::sqrt(-3.0 / A);
            if (arg > 1.0)  arg = 1.0;
            if (arg < -1.0) arg = -1.0;
            double phi = std::acos(arg) / 3.0;
            for (int k = 0; k < 3; ++k) {
                r.re[k] = m * std::cos(phi - 2.0 * PI * k / 3.0) - p / 3.0;
                r.im[k] = 0.0;
            }
        }
    }

    for (int k = 0; k < 3; ++k)
        r.mod[k] = std::sqrt(r.re[k] * r.re[k] + r.im[k] * r.im[k]);

    // Округляем до 9 знаков перед сравнением: корень на окружности
    // из-за округления может выйти 1 +/- 1e-16
    r.stable = true;
    for (int k = 0; k < 3; ++k) {
        double m = std::round(r.mod[k] * 1e9) / 1e9;
        if (m >= 1.0) r.stable = false;
    }
    return r;
}

// Коротко: устойчива ли модель 1.6 с такими коэффициентами
inline bool stable16(double a1, double a2, double a3) {
    return cubicRoots(a1, a2, a3).stable;
}

// Отчёт: уравнение, корни, модули, вердикт.
// Поток out: в main - std::cout, в тестах - строковый поток.
inline void printStabilityCheck(double a1, double a2, double a3, std::ostream& out) {
    CubicRoots r = cubicRoots(a1, a2, a3);
    out << "Stability check (model 1.6):" << std::endl
        << "  z^3 - (" << a1 << ")*z^2 - (" << a2 << ")*z - (" << a3 << ") = 0" << std::endl;
    out << std::fixed << std::setprecision(4);
    for (int k = 0; k < 3; ++k) {
        out << "  z" << k + 1 << " = " << r.re[k];
        if (r.im[k] != 0.0)
            out << (r.im[k] > 0.0 ? " + j" : " - j") << std::fabs(r.im[k]);
        out << ",   |z" << k + 1 << "| = " << r.mod[k] << std::endl;
    }
    if (r.stable)
        out << "  All roots are inside the unit circle -> STABLE." << std::endl;
    else
        out << "  WARNING: a root is on or outside the unit circle -> UNSTABLE,"
            << " y(t) will not settle!" << std::endl;
}
