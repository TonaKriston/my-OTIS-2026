#include "../src/models.h"
#include "../src/stability_analyzer.h"

#include <gtest/gtest.h>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

class LinearSimulationTest : public testing::TestWithParam<std::tuple<double, double, double>>
{
};

TEST_P(LinearSimulationTest, CalculatesExpectedFirstSteps)
{
    const auto& [a1, a2, b] = GetParam();
    Linear model(a1, a2, b, 1, 4, InputType::Step, 2.0);
    model.run();
    const auto& y = model.outputs();
    EXPECT_DOUBLE_EQ(y[0], 0.0);
    EXPECT_DOUBLE_EQ(y[1], 2.0 * b);
    EXPECT_DOUBLE_EQ(y[2], a1 * y[1] + 2.0 * b);
    EXPECT_DOUBLE_EQ(y[3], a1 * y[2] + a2 * y[1] + 2.0 * b);
}

INSTANTIATE_TEST_SUITE_P(Coefficients, LinearSimulationTest,
    testing::Values(std::make_tuple(0.2, 0.1, 0.5),
                    std::make_tuple(-0.3, 0.2, 1.0),
                    std::make_tuple(0.0, 0.5, -0.25)));

TEST(ModelInputsTest, CreatesStepImpulseAndHarmonicSignals){
    using enum InputType;
    Linear step(0, 0, 0, 1, 3, InputType::Step, 4);
    Linear impulse(0, 0, 0, 1, 3, InputType::Impulse);
    Linear harmonic(0, 0, 0, 1, 3, InputType::Harmonic);
    EXPECT_EQ(step.inputs(), (std::vector<double>{4, 4, 4}));
    EXPECT_EQ(impulse.inputs(), (std::vector<double>{1, 0, 0}));
    EXPECT_NEAR(harmonic.inputs()[1], std::sin(1.0), 1e-12);
}

TEST(ModelValidationTest, RejectsInvalidParameters)
{
    EXPECT_THROW(Linear(0, 0, 0, 0, 2, InputType::Step), std::invalid_argument);
    EXPECT_THROW(NonLinear(0, 0, -1, 2, InputType::Step), std::invalid_argument);
    EXPECT_THROW(Differential(1, 0, 2, InputType::Step), std::invalid_argument);
    EXPECT_THROW(Linear(0, 0, 0, 1, 0, InputType::Step), std::invalid_argument);
    EXPECT_THROW(Linear(0, 0, 0, 1, -1, InputType::Step), std::invalid_argument);
}

TEST(NonLinearTest, RelayUsesHysteresisBoundaries)
{
    NonLinear positive(0, 1, 0.5, 2, InputType::Step, 0.5);
    positive.run();
    EXPECT_DOUBLE_EQ(positive.outputs()[1], -1.0);

    NonLinear negative(0, 1, 0.5, 2, InputType::Step, -0.5);
    negative.run();
    EXPECT_DOUBLE_EQ(negative.outputs()[1], -1.0);
}

TEST(DifferentialTest, UsesEulerIntegration)
{
    Differential model(2.0, 0.1, 3, InputType::Step, 1.0);
    model.run();
    EXPECT_NEAR(model.outputs()[1], 0.1 * 2.0 * std::tanh(1.0), 1e-12);
    EXPECT_NEAR(model.outputs()[2], 2.0 * model.outputs()[1], 1e-12);
}

TEST(ModelOutputTest, DisplaysAndSavesResults)
{
    Differential model(2.0, 0.1, 3, InputType::Step, 1.0);
    model.run();

    std::ostringstream output;
    auto* previousBuffer = std::cout.rdbuf(output.rdbuf());
    model.showResult();
    std::cout.rdbuf(previousBuffer);

    EXPECT_NE(output.str().find("|        u"), std::string::npos);

    const std::string outputPath = "coverage-results.csv";
    EXPECT_TRUE(model.saveCsv(outputPath));

    std::ifstream file(outputPath);
    EXPECT_TRUE(file.good());
    EXPECT_EQ(std::string(
        std::istreambuf_iterator<char>(file), {} ).substr(0, 6), "t;u;y\n");
    file.close();
    std::remove(outputPath.c_str());

    Linear linear(0.2, 0.1, 0.5, 1, 2, InputType::Step, 1.0);
    linear.run();
    EXPECT_TRUE(linear.saveCsv("linear-results.csv"));
    std::remove("linear-results.csv");
}

TEST(ModelOutputTest, ReportsFileErrors)
{
    Linear model(0.2, 0.1, 0.5, 1, 2, InputType::Step, 1.0);
    EXPECT_FALSE(model.saveCsv("missing-directory/results.csv"));
}

TEST(NonLinearTest, RelaySelectsPositiveAndNegativeRegions)
{
    NonLinear positive(0.0, 1.0, 0.5, 2, InputType::Step, 1.0);
    positive.run();
    EXPECT_DOUBLE_EQ(positive.outputs()[1], 1.0);

    NonLinear hysteresis(0.0, 1.0, 0.5, 2, InputType::Step, -1.0);
    hysteresis.run();
    EXPECT_DOUBLE_EQ(hysteresis.outputs()[1], -1.0);
}

TEST(StabilityAnalyzerTest, StableFirstOrderEquivalent)
{
    EXPECT_TRUE(StabilityAnalyzer::isStable(0.5, 0.0, 1));
    EXPECT_FALSE(StabilityAnalyzer::isStable(1.0, 0.0, 1));
    EXPECT_FALSE(StabilityAnalyzer::isStable(1.2, 0.0, 1));
}

TEST(StabilityAnalyzerTest, DetectsDelayedModelRoots)
{
    EXPECT_TRUE(StabilityAnalyzer::isStable(0.2, 0.1, 2));
    EXPECT_FALSE(StabilityAnalyzer::isStable(0.0, 1.1, 2));
}

TEST(StabilityAnalyzerTest, RejectsZeroDelay)
{
    EXPECT_THROW(StabilityAnalyzer::isStable(0, 0, 0), std::invalid_argument);
}

TEST(StabilityAnalyzerTest, HandlesSlowConvergence)
{
    EXPECT_EQ(StabilityAnalyzer::roots(100.0, 100.0, 1).size(), 2U);
}
