#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cmath>
#include <sstream>
#include <iostream>
#include "../src/Models.h"

TEST(StabilityAnalyzerTest, ReturnsTrueForStableCoefficients) {
    EXPECT_TRUE(Model1_7::checkStability(0.0));
    EXPECT_TRUE(Model1_7::checkStability(0.5));
    EXPECT_TRUE(Model1_7::checkStability(-0.5));
    EXPECT_TRUE(Model1_7::checkStability(0.999));
    EXPECT_TRUE(Model1_7::checkStability(-0.999));
}

TEST(StabilityAnalyzerTest, ReturnsFalseOnBoundaryOfUnitCircle) {
    EXPECT_FALSE(Model1_7::checkStability(1.0));
    EXPECT_FALSE(Model1_7::checkStability(-1.0));
}

TEST(StabilityAnalyzerTest, ReturnsFalseForUnstableCoefficients) {
    EXPECT_FALSE(Model1_7::checkStability(1.5));
    EXPECT_FALSE(Model1_7::checkStability(-2.0));
    EXPECT_FALSE(Model1_7::checkStability(100.0));
}

TEST(StabilityAnalyzerTest, PrintsWarningWhenConstructedWithUnstableCoeff) {
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    Model1_7 unstableModel(1.2, 1.0, 0.5, 0.2);

    std::cout.rdbuf(oldCout);
    EXPECT_FALSE(unstableModel.isStable());
    EXPECT_NE(buffer.str().find("[WARNING]"), std::string::npos);
}

TEST(Model1_7Test, ManualCalculationMultiStepHistory) {
    Model1_7 model(0.5, 2.0, 1.0, 0.5);

    EXPECT_DOUBLE_EQ(model.nextStep(1.0), 2.0);

    EXPECT_DOUBLE_EQ(model.nextStep(2.0), 6.0);

    EXPECT_DOUBLE_EQ(model.nextStep(3.0), 11.5);

    model.reset();
    EXPECT_DOUBLE_EQ(model.nextStep(1.0), 2.0);
}


TEST(Model2_9Test, BoundaryAndSignNonLinearity) {
    Model2_9 model(0.5, 2.0);

    EXPECT_DOUBLE_EQ(model.nextStep(0.0), 0.0);

    EXPECT_DOUBLE_EQ(model.nextStep(4.0), 4.0);

    EXPECT_DOUBLE_EQ(model.nextStep(-9.0), -4.0);

    model.reset();
    EXPECT_DOUBLE_EQ(model.nextStep(0.0), 0.0);
}

TEST(PhysicalInvariantsTest, ZeroInputKeepsZeroStateAtRest) {
    Model1_7 m1(0.8, 1.0, 0.5, 0.2);
    Model2_9 m2(0.8, 1.5);

    for (int i = 0; i < 10; ++i) {
        EXPECT_DOUBLE_EQ(m1.nextStep(0.0), 0.0);
        EXPECT_DOUBLE_EQ(m2.nextStep(0.0), 0.0);
    }
}

TEST(Model3_1Test, EulerMethodApproximatesAnalyticalDecay) {
    double a = 0.5;
    double dt = 0.001;
    Model3_1 model(a, dt);

    double y = 1.0;
    int steps = 1000;
    for (int i = 0; i < steps; ++i) {
        y = model.nextStep(0.0);
    }

    double analytical = std::exp(-a * 1.0);
    EXPECT_NEAR(y, analytical, 1e-3);

    model.reset();
    EXPECT_DOUBLE_EQ(model.nextStep(0.0), 1.0 - dt * a * 1.0);
}

struct ConvergenceParam {
    double a;
    double b1;
    double b2;
    double b3;
    double stepInput;
    double expectedSteadyState;
};

class Model1_7ConvergenceTest : public ::testing::TestWithParam<ConvergenceParam> {};

TEST_P(Model1_7ConvergenceTest, ConvergesToAnalyticalSteadyStateOnStepInput) {
    ConvergenceParam p = GetParam();
    Model1_7 model(p.a, p.b1, p.b2, p.b3);

    double y = 0.0;
    for (int tau = 0; tau < 200; ++tau) {
        y = model.nextStep(p.stepInput);
    }

    EXPECT_NEAR(y, p.expectedSteadyState, 1e-4);
}

INSTANTIATE_TEST_SUITE_P(
    SteadyStateTests,
    Model1_7ConvergenceTest,
    ::testing::Values(
        ConvergenceParam{ 0.5, 1.0, 0.5, 0.2, 1.0, 3.4 },
        ConvergenceParam{ 0.2, 0.5, 0.3, 0.2, 2.0, 2.5 },
        ConvergenceParam{ -0.5, 1.0, 0.0, 0.0, 1.0, 1.0 / 1.5 }
    )
);

class MockSignalGenerator : public ISignalGenerator {
public:
    MOCK_METHOD(double, generate, (int tau), (const, override));
};

TEST(SignalMockTest, UsesMockGeneratorForControlRegimes) {
    MockSignalGenerator mockGen;
    Model1_7 model(0.5, 2.0, 1.0, 0.0);

    EXPECT_CALL(mockGen, generate(0)).Times(1).WillOnce(::testing::Return(1.0));
    EXPECT_CALL(mockGen, generate(1)).Times(1).WillOnce(::testing::Return(0.0));
    EXPECT_CALL(mockGen, generate(2)).Times(1).WillOnce(::testing::Return(0.0));

    EXPECT_DOUBLE_EQ(model.stepWithGenerator(mockGen, 0), 2.0);
    EXPECT_DOUBLE_EQ(model.stepWithGenerator(mockGen, 1), 2.0);
    EXPECT_DOUBLE_EQ(model.stepWithGenerator(mockGen, 2), 1.0);
}

TEST(SignalSubstitutionTest, StandardSignalGeneratorModes) {
    StandardSignalGenerator stepGen(0);
    StandardSignalGenerator impulseGen(1);
    StandardSignalGenerator harmonicGen(2);
    StandardSignalGenerator unknownGen(99);

    EXPECT_DOUBLE_EQ(stepGen.generate(0), 1.0);
    EXPECT_DOUBLE_EQ(stepGen.generate(5), 1.0);

    EXPECT_DOUBLE_EQ(impulseGen.generate(0), 1.0);
    EXPECT_DOUBLE_EQ(impulseGen.generate(1), 0.0);

    EXPECT_DOUBLE_EQ(harmonicGen.generate(2), std::sin(2));
    EXPECT_DOUBLE_EQ(unknownGen.generate(0), 0.0);
}