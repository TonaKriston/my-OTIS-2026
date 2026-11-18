#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "LinearModel13.h"
#include "NonlinearModel2_10.h"
#include "DiffEqModel3_2.h"

TEST(LinearModelTest, ManualCalculation) {
    LinearModel13 model(0.5, 0.1, 2.0);
    EXPECT_DOUBLE_EQ(model.step(1.0), 2.0);
    EXPECT_DOUBLE_EQ(model.step(1.0), 3.0);
}
TEST(NonlinearModelTest, BoundaryValues) {
    NonlinearModel2_10 model(1.0, 1.0);
    EXPECT_DOUBLE_EQ(model.step(10.0), 1000.0); 
    model.reset();
    EXPECT_DOUBLE_EQ(model.step(0.0), 0.0);
}
TEST(DiffEqModelTest, EulerVsAnalytical) {
    DiffEqModel3_2 model(2.0, 0.1);
    for (int i = 0; i < 5; ++i) model.step(1.0);
    EXPECT_NEAR(model.step(1.0), 1.2, 1e-5); 
}
TEST(InvariantsTest, ZeroInputZeroState) {
    LinearModel13 lin(0.5, 0.2, 1.0);
    NonlinearModel2_10 nonlin(0.5, 0.2);
    DiffEqModel3_2 diff(0.5, 0.1);
    lin.step(1.0); diff.step(1.0);
    lin.reset(); diff.reset(); nonlin.reset();
    EXPECT_DOUBLE_EQ(lin.step(0.0), 0.0);
    EXPECT_DOUBLE_EQ(nonlin.step(0.0), 0.0);
    EXPECT_DOUBLE_EQ(diff.step(0.0), 0.0);
}

struct ModelTestParam { double a1, a2, b; std::vector<double> inputs, expected; };
class LinearModelParamTest : public ::testing::TestWithParam<ModelTestParam> {};

TEST_P(LinearModelParamTest, StepSimulationConvergence) {
    auto param = GetParam();
    LinearModel13 model(param.a1, param.a2, param.b);
    for (size_t i = 0; i < param.inputs.size(); ++i) {
        EXPECT_NEAR(model.step(param.inputs[i]), param.expected[i], 1e-5);
    }
}
INSTANTIATE_TEST_SUITE_P(SimulationSteps, LinearModelParamTest, ::testing::Values(
    ModelTestParam{0.5, 0.1, 1.0, {1.0, 1.0, 1.0}, {1.0, 1.5, 1.85}},
    ModelTestParam{0.5, 0.1, 1.0, {1.0, 0.0, 0.0}, {1.0, 0.5, 0.35}},
    ModelTestParam{0.5, 0.0, 2.0, {0.0, 0.5, 1.0}, {0.0, 1.0, 2.5}}
));