#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <cmath>
#include "models.h"

using ::testing::Pointwise;
using ::testing::DoubleNear;

// ---------- Общая структура параметра ----------
struct ModelCase {
    double a1;          // для линейной: a1
    double a2;          // для линейной: a2
    double b;           // для всех: b
    double dt;          // для дифференциальной: dt
    int    signalType;  // 0=step, 1=impulse, 2=harmonic
    std::vector<double> expected; // ожидаемые y
};

// ============================================================
// 1. Линейная модель
//    y[tau+1] = a1*y[tau] + a2*y[tau-1] + b*u[tau]
// ============================================================
class LinearModelParamTest : public ::testing::TestWithParam<ModelCase> {};
    

TEST_P(LinearModelParamTest, MatchesReferenceTrajectory) {
    const auto& p = GetParam(); //задание параметров тестируемой модели
    LinearModel model(p.a1, p.a2, p.b); //создание объекта с заданными параметрами
    model.reset();

    for (size_t tau = 0; tau < p.expected.size(); ++tau) {
        double u = generateSignal(p.signalType, static_cast<int>(tau));
        double y = model.step(u);
        // Точность ослаблена до 1e-4, чтобы мне было не лень вбивать значения
        EXPECT_NEAR(y, p.expected[tau], 1e-4)
            << "Mismatch at tau=" << tau
            << " signal=" << p.signalType;
    }
}

INSTANTIATE_TEST_SUITE_P(
    Linear_Step, LinearModelParamTest,
    ::testing::Values(
        ModelCase{0.5, 0.2, 1.0, 0.0, 0, {0.0, 1.0, 2.5, 4.45, 6.725}}
    ));

INSTANTIATE_TEST_SUITE_P(
    Linear_Impulse, LinearModelParamTest,
    ::testing::Values(
        //первое значение y0 = 1.0, так как step возвращает y_next
        ModelCase{0.5, 0.2, 1.0, 0.0, 1, {1.0, 0.5, 0.45, 0.325, 0.2525}}
    ));

INSTANTIATE_TEST_SUITE_P(
    Linear_Harmonic, LinearModelParamTest,
    ::testing::Values(
        ModelCase{0.5, 0.2, 1.0, 0.0, 2,
                  {0.0, 0.84147, 1.33003, 0.97443, -0.00358}}
    ));

// ============================================================
// 2. Нелинейная модель
//    y[tau+1] = a*tanh(y[tau]) + b*u[tau]^3
// ============================================================
class NonlinearModelParamTest
    : public ::testing::TestWithParam<ModelCase> {};

TEST_P(NonlinearModelParamTest, MatchesReferenceTrajectory) {
    const auto& p = GetParam();
    NonlinearModel model(p.a1, p.b); // a1=a, a2 не используется
    model.reset();

    for (size_t tau = 0; tau < p.expected.size(); ++tau) {
        double u = generateSignal(p.signalType, static_cast<int>(tau));
        double y = model.step(u);
        EXPECT_NEAR(y, p.expected[tau], 1e-4)
            << "Mismatch at tau=" << tau;
    }
}


INSTANTIATE_TEST_SUITE_P(
    Nonlinear_Step, NonlinearModelParamTest,
    ::testing::Values(
        ModelCase{0.5, 0.0, 0.1, 0.0, 0, {0.0, 0.1, 0.84983, 3.04545}}
    ));

INSTANTIATE_TEST_SUITE_P(
    Nonlinear_Impulse, NonlinearModelParamTest,
    ::testing::Values(
        ModelCase{0.5, 0.0, 0.1, 0.0, 1, {0.1, 0.04984, 0.02492, 0.01245}}
    ));

INSTANTIATE_TEST_SUITE_P(
    Nonlinear_Harmonic, NonlinearModelParamTest,
    ::testing::Values(
        ModelCase{0.5, 0.0, 0.1, 0.0, 2, {0.0, 0.05958, 0.10494, 0.05255}}
    ));

// ============================================================
// 3. Дифференциальная модель (Эйлер)
//    y[tau+1] = y[tau] + b*u[tau]*dt
// ============================================================
class DifferentialModelParamTest
    : public ::testing::TestWithParam<ModelCase> {};

TEST_P(DifferentialModelParamTest, MatchesReferenceTrajectory) {
    const auto& p = GetParam();
    DifferentialModel model(p.b, p.dt);
    model.reset();

    for (size_t tau = 0; tau < p.expected.size(); ++tau) {
        double u = generateSignal(p.signalType, static_cast<int>(tau));
        double y = model.step(u);
        EXPECT_NEAR(y, p.expected[tau], 1e-4)
            << "Mismatch at tau=" << tau;
    }
}

INSTANTIATE_TEST_SUITE_P(
    Differential_Step, DifferentialModelParamTest,
    ::testing::Values(
        ModelCase{0.0, 0.0, 1.0, 0.5, 0, {0.0, 0.5, 1.5, 3.0}}
    ));

INSTANTIATE_TEST_SUITE_P(
    Differential_Impulse, DifferentialModelParamTest,
    ::testing::Values(
        ModelCase{0.0, 0.0, 1.0, 0.5, 1, {0.5, 0.5, 0.5, 0.5}}
    ));

INSTANTIATE_TEST_SUITE_P(
    Differential_Harmonic, DifferentialModelParamTest,
    ::testing::Values(
        ModelCase{0.0, 0.0, 1.0, 0.5, 2, {0.0, 0.42073, 0.87538, 0.94589}}
    ));