#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cmath>
#include <memory>
#include <tuple>

#include "InputSignal.h"
#include "Model.h"
#include "Model1_3.h"
#include "Model2_5.h"
#include "Model3_7.h"
#include "Simulation.h"
#include <stdexcept>

// Параметризованная проверка шага расчёта линейной модели для разных входных данных.
class Model13StepTest : public ::testing::TestWithParam<std::tuple<double, double, double, double, double, double>>
{};

TEST_P(Model13StepTest, CalculatesExpectedNextValue)
{
    const auto [a1, a2, b, y, yPrev, u] = GetParam();
    const Model1_3 model(a1, a2, b);
    EXPECT_DOUBLE_EQ(model.calculateNext(y, yPrev, u, 1.0), a1 * y + a2 * yPrev + b * u);
}

INSTANTIATE_TEST_SUITE_P(
    VariousInputs, Model13StepTest,
    ::testing::Values(
        std::make_tuple(0.5, 0.2, 1.0, 2.0, 1.0, 3.0),
        std::make_tuple(0.0, 0.0, 2.0, 0.0, 0.0, 0.0),
        std::make_tuple(-0.3, 0.1, 0.7, -2.0, 4.0, 1.5),
        std::make_tuple(0.8, -0.1, 0.25, 1.0, 2.0, -4.0)));

TEST(ModelNameTest, ModelsReturnNames)
{
    EXPECT_THAT(Model1_3(0.5, 0.2, 1.0).getName(), ::testing::HasSubstr("Model 1.3"));
    EXPECT_THAT(Model2_5(0.5, 1.0).getName(), ::testing::HasSubstr("Model 2.5"));
    EXPECT_THAT(Model3_7(0.2, 1.0).getName(), ::testing::HasSubstr("Model 3.7"));
}

TEST(Model25Test, ZeroInputKeepsZeroState)
{
    const Model2_5 model(0.7, 10.0);
    EXPECT_DOUBLE_EQ(model.calculateNext(0.0, 0.0, 0.0, 1.0), 0.0);
}

TEST(Model25Test, PositiveAndNegativeInputsUseOddNonlinearity)
{
    const Model2_5 model(0.0, 2.0);
    const double positive = model.calculateNext(0.0, 0.0, 1.0, 1.0);
    const double negative = model.calculateNext(0.0, 0.0, -1.0, 1.0);
    EXPECT_NEAR(positive, 2.0 * (1.0 - std::exp(-1.0)), 1e-12);
    EXPECT_NEAR(negative, -positive, 1e-12);
}

TEST(Model37Test, EulerStepMatchesFormula)
{
    const Model3_7 model(0.0, 2.0);
    const double y = 3.0, u = 4.0, dt = 0.1;
    EXPECT_NEAR(model.calculateNext(y, 0.0, u, dt),
                y + dt * (-std::exp(0.0) * y + 2.0 * u), 1e-12);
}

TEST(Model37Test, ZeroStepDoesNotChangeState)
{
    const Model3_7 model(0.6, 3.0);
    EXPECT_DOUBLE_EQ(model.calculateNext(7.5, 0.0, 2.0, 0.0), 7.5);
}

TEST(InputSignalTest, StepSignalIsConstant)
{
    EXPECT_DOUBLE_EQ(getInput(SignalType::Step, 0, 2.5), 2.5);
    EXPECT_DOUBLE_EQ(getInput(SignalType::Step, 100, 2.5), 2.5);
}

TEST(InputSignalTest, ImpulseIsNonzeroOnlyAtStart)
{
    EXPECT_DOUBLE_EQ(getInput(SignalType::Impulse, 0, 3.0), 3.0);
    EXPECT_DOUBLE_EQ(getInput(SignalType::Impulse, 1, 3.0), 0.0);
}

TEST(InputSignalTest, HarmonicSignalUsesSine)
{
    EXPECT_NEAR(getInput(SignalType::Harmonic, 0, 3.0), 0.0, 1e-12);
    EXPECT_NEAR(getInput(SignalType::Harmonic, 1, 3.0), 3.0 * std::sin(1.0), 1e-12);
}


TEST(InputSignalTest, ImpulseIsZeroForAnyNonzeroStepIndex)
{
    EXPECT_DOUBLE_EQ(getInput(SignalType::Impulse, -1, 4.0), 0.0);
    EXPECT_DOUBLE_EQ(getInput(SignalType::Impulse, 5, 4.0), 0.0);
}

TEST(InputSignalTest, ZeroAmplitudeProducesZeroForAllSignals)
{
    for (SignalType signal : {SignalType::Step, SignalType::Impulse, SignalType::Harmonic})
        for (int tau : {-1, 0, 1, 10})
            EXPECT_DOUBLE_EQ(getInput(signal, tau, 0.0), 0.0);
}

TEST(SimulationTest, SimulatesKnownLinearSequenceAndKeepsRows)
{
    const Model1_3 model(0.5, 0.0, 1.0);
    const auto rows = simulate(model, SignalType::Step, 2.0, 3, 0.0);
    ASSERT_EQ(rows.size(), 4u);
    EXPECT_EQ(rows.front().tau, 0);
    EXPECT_DOUBLE_EQ(rows.front().y, 0.0);
    EXPECT_DOUBLE_EQ(rows[1].y, 2.0);
    EXPECT_DOUBLE_EQ(rows[2].y, 3.0);
    EXPECT_DOUBLE_EQ(rows[3].y, 3.5);
    EXPECT_EQ(rows.back().tau, 3);
}

TEST(SimulationTest, ImpulseOnlyAffectsFirstCalculatedStep)
{
    const Model1_3 model(0.0, 0.0, 1.0);
    const auto rows = simulate(model, SignalType::Impulse, 5.0, 2, 0.0);
    ASSERT_EQ(rows.size(), 3u);
    EXPECT_DOUBLE_EQ(rows[1].y, 5.0);
    EXPECT_DOUBLE_EQ(rows[2].y, 0.0);
}

TEST(SimulationTest, RejectsInvalidStepCount)
{
    const Model1_3 model(0.5, 0.2, 1.0);
    EXPECT_THROW(simulate(model, SignalType::Step, 1.0, 0, 0.0), std::invalid_argument);
    EXPECT_THROW(simulate(model, SignalType::Step, 1.0, -1, 0.0), std::invalid_argument);
}


TEST(InputSignalTest, UnknownSignalTypeFallsBackToZero)
{
    EXPECT_DOUBLE_EQ(getInput(static_cast<SignalType>(99), 2, 5.0), 0.0);
}

TEST(SimulationTest, ZeroInitialStateAndZeroAmplitudeStayFinite)
{
    const Model3_7 model(0.2, 1.0);
    const auto rows = simulate(model, SignalType::Harmonic, 0.0, 3, 0.0, 0.25);
    ASSERT_EQ(rows.size(), 4u);
    for (const auto& row : rows) {
        EXPECT_DOUBLE_EQ(row.u, 0.0);
        EXPECT_DOUBLE_EQ(row.y, 0.0);
    }
}

TEST(SimulationTest, UsesIntegrationStepForContinuousModel)
{
    const Model3_7 model(0.0, 2.0);
    const auto rows = simulate(model, SignalType::Step, 1.0, 1, 0.0, 0.5);
    ASSERT_EQ(rows.size(), 2u);
    EXPECT_DOUBLE_EQ(rows[1].y, 1.0);
}


// Tests for the public interface declared in Model1_3.h.
TEST(Model13HeaderTest, ConstructorStoresCoefficientsUsedInCalculation)
{
    // Геттеров в Model1_3 (task_01) нет, поэтому проверяем коэффициенты по поведению:
    // каждый из a1, a2, b влияет на результат независимо.
    const Model1_3 model(0.25, -0.75, 2.5);
    EXPECT_DOUBLE_EQ(model.calculateNext(1.0, 0.0, 0.0, 1.0), 0.25);
    EXPECT_DOUBLE_EQ(model.calculateNext(0.0, 1.0, 0.0, 1.0), -0.75);
    EXPECT_DOUBLE_EQ(model.calculateNext(0.0, 0.0, 1.0, 1.0), 2.5);
}

TEST(Model13HeaderTest, CalculateNextIgnoresIntegrationStep)
{
    const Model1_3 model(0.5, 0.25, 2.0);
    const double expected = 0.5 * 4.0 + 0.25 * 2.0 + 2.0 * 3.0;
    EXPECT_DOUBLE_EQ(model.calculateNext(4.0, 2.0, 3.0, 0.0), expected);
    EXPECT_DOUBLE_EQ(model.calculateNext(4.0, 2.0, 3.0, 10.0), expected);
}

TEST(Model13HeaderTest, WorksThroughBaseClassInterface)
{
    const Model1_3 concrete(0.4, -0.1, 1.5);
    const Model& model = concrete;
    EXPECT_NEAR(model.calculateNext(2.0, 3.0, 4.0, 1.0), 6.5, 1e-12);
    EXPECT_STREQ(model.getName(), "Model 1.3 - Second-Order Linear Model");
}

// ---------------------------------------------------------------------------
// Параметризованные тесты сходимости (TEST_P): устойчивая модель при ступенчатом
// воздействии выходит на установившееся значение, найденное аналитически.
// ---------------------------------------------------------------------------

// (a1, a2, b, A): y_inf = b*A / (1 - a1 - a2)
class Model13ConvergenceTest
    : public ::testing::TestWithParam<std::tuple<double, double, double, double>>
{};

TEST_P(Model13ConvergenceTest, StepResponseConvergesToSteadyState)
{
    const auto [a1, a2, b, amplitude] = GetParam();
    const Model1_3 model(a1, a2, b);
    const auto rows = simulate(model, SignalType::Step, amplitude, 400, 0.0);
    EXPECT_NEAR(rows.back().y, b * amplitude / (1.0 - a1 - a2), 1e-6);
}

INSTANTIATE_TEST_SUITE_P(
    StableCoefficients, Model13ConvergenceTest,
    ::testing::Values(
        std::make_tuple(0.5, 0.2, 1.0, 1.0),
        std::make_tuple(0.3, -0.2, 2.0, 1.5),
        std::make_tuple(0.0, 0.5, 1.0, 2.0),
        std::make_tuple(-0.4, 0.1, 0.5, 4.0)));

// (a, b, A): y_inf = b*sign(A)*(1 - exp(-|A|)) / (1 - a)
class Model25ConvergenceTest
    : public ::testing::TestWithParam<std::tuple<double, double, double>>
{};

TEST_P(Model25ConvergenceTest, StepResponseConvergesToSteadyState)
{
    const auto [a, b, amplitude] = GetParam();
    const Model2_5 model(a, b);
    const auto rows = simulate(model, SignalType::Step, amplitude, 400, 0.0);
    const double sign = amplitude > 0.0 ? 1.0 : -1.0;
    EXPECT_NEAR(rows.back().y,
                b * sign * (1.0 - std::exp(-std::abs(amplitude))) / (1.0 - a), 1e-6);
}

INSTANTIATE_TEST_SUITE_P(
    StableCoefficients, Model25ConvergenceTest,
    ::testing::Values(
        std::make_tuple(0.5, 1.0, 1.0),
        std::make_tuple(0.9, 2.0, 3.0),
        std::make_tuple(-0.5, 1.0, 2.0),
        std::make_tuple(0.5, 1.0, -1.0)));

// (a, b, A, dt): y_inf = b*A / exp(a), метод Эйлера устойчив при dt*exp(a) < 2.
class Model37ConvergenceTest
    : public ::testing::TestWithParam<std::tuple<double, double, double, double>>
{};

TEST_P(Model37ConvergenceTest, EulerStepResponseConvergesToSteadyState)
{
    const auto [a, b, amplitude, dt] = GetParam();
    const Model3_7 model(a, b);
    const auto rows = simulate(model, SignalType::Step, amplitude, 5000, 0.0, dt);
    EXPECT_NEAR(rows.back().y, b * amplitude / std::exp(a), 1e-6);
}

INSTANTIATE_TEST_SUITE_P(
    StableCoefficients, Model37ConvergenceTest,
    ::testing::Values(
        std::make_tuple(0.0, 1.0, 1.0, 0.1),
        std::make_tuple(0.5, 2.0, 1.0, 0.05),
        std::make_tuple(-0.5, 1.0, 3.0, 0.2)));

// ---------------------------------------------------------------------------
// Google Mock: подмена модели проверяет, что simulate() передаёт в calculateNext
// корректные y(t), y(t-1), u(t), dt для разных режимов управления.
// ---------------------------------------------------------------------------

class MockModel : public Model
{
public:
    MOCK_METHOD(double, calculateNext, (double, double, double, double), (const, override));
    MOCK_METHOD(const char*, getName, (), (const, override));
};

TEST(SimulationMockTest, StepSignalPassesStateHistoryAndStepToModel)
{
    MockModel model;
    {
        ::testing::InSequence sequence;
        EXPECT_CALL(model, calculateNext(1.0, 1.0, 2.0, 0.5)).WillOnce(::testing::Return(10.0));
        EXPECT_CALL(model, calculateNext(10.0, 1.0, 2.0, 0.5)).WillOnce(::testing::Return(20.0));
        EXPECT_CALL(model, calculateNext(20.0, 10.0, 2.0, 0.5)).WillOnce(::testing::Return(30.0));
    }

    const auto rows = simulate(model, SignalType::Step, 2.0, 3, 1.0, 0.5);

    ASSERT_EQ(rows.size(), 4u);
    EXPECT_DOUBLE_EQ(rows[1].y, 10.0);
    EXPECT_DOUBLE_EQ(rows[2].y, 20.0);
    EXPECT_DOUBLE_EQ(rows[3].y, 30.0);
}

TEST(SimulationMockTest, ImpulseSignalIsNonzeroOnlyOnFirstCall)
{
    MockModel model;
    {
        ::testing::InSequence sequence;
        EXPECT_CALL(model, calculateNext(::testing::_, ::testing::_, 5.0, ::testing::_))
            .WillOnce(::testing::Return(0.0));
        EXPECT_CALL(model, calculateNext(::testing::_, ::testing::_, 0.0, ::testing::_))
            .Times(2)
            .WillRepeatedly(::testing::Return(0.0));
    }

    simulate(model, SignalType::Impulse, 5.0, 3, 0.0);
}

TEST(SimulationMockTest, HarmonicSignalFeedsSineSamplesToModel)
{
    MockModel model;
    {
        ::testing::InSequence sequence;
        EXPECT_CALL(model, calculateNext(::testing::_, ::testing::_,
                                         ::testing::DoubleNear(0.0, 1e-12), ::testing::_))
            .WillOnce(::testing::Return(0.0));
        EXPECT_CALL(model, calculateNext(::testing::_, ::testing::_,
                                         ::testing::DoubleNear(2.0 * std::sin(1.0), 1e-12),
                                         ::testing::_))
            .WillOnce(::testing::Return(0.0));
        EXPECT_CALL(model, calculateNext(::testing::_, ::testing::_,
                                         ::testing::DoubleNear(2.0 * std::sin(2.0), 1e-12),
                                         ::testing::_))
            .WillOnce(::testing::Return(0.0));
    }

    simulate(model, SignalType::Harmonic, 2.0, 3, 0.0);
}

TEST(SimulationMockTest, InvalidStepCountNeverCallsModel)
{
    MockModel model;
    EXPECT_CALL(model, calculateNext(::testing::_, ::testing::_, ::testing::_, ::testing::_))
        .Times(0);
    EXPECT_THROW(simulate(model, SignalType::Step, 1.0, 0, 0.0), std::invalid_argument);
}
