#include <gtest/gtest.h>
#include <cmath>
#include <functional>
#include <memory>
#include "../../task_01/src/model19.h"
#include "../../task_01/src/model23.h"
#include "../../task_01/src/model37.h"

TEST(Model19, ManualSteps) {
    Model19 m(0.5, 0.2, 0.1, 1.0);
    EXPECT_NEAR(m.nextStep(1.0), 1.0, 0.000000001);
    EXPECT_NEAR(m.nextStep(1.0), 1.5, 0.000000001);
    EXPECT_NEAR(m.nextStep(1.0), 1.95, 0.000000001);
}
TEST(Model23, ManualSteps) {
    Model23 m(0.5, 1.0, 0.2);
    EXPECT_NEAR(m.nextStep(1.0), 0.8, 0.000000001);
    EXPECT_NEAR(m.nextStep(1.0), 1.2, 0.000000001);
}
TEST(Model37, ManualSteps) {
    Model37 m(0.0, 1.0, 0.1);
    EXPECT_NEAR(m.nextStep(1.0), 0.1, 0.000000001);
    EXPECT_NEAR(m.nextStep(1.0), 0.19, 0.000000001);
}

// границы зоны нечувствительности
TEST(Model23, DeadZoneBoundary) {
    Model23 m(0.5, 1.0, 0.2);
    EXPECT_DOUBLE_EQ(m.nextStep(0.2), 0.0);
    Model23 n(0.5, 1.0, 0.2);
    EXPECT_DOUBLE_EQ(n.nextStep(-0.2), 0.0);
}

// покой при u = 0 
TEST(Models, RestState) {
    Model19 a(0.5, 0.2, 0.1, 1.0);
    Model23 b(0.5, 1.0, 0.2);
    Model37 c(0.0, 1.0, 0.1);
    EXPECT_DOUBLE_EQ(a.nextStep(0.0), 0.0);
    EXPECT_DOUBLE_EQ(b.nextStep(0.0), 0.0);
    EXPECT_DOUBLE_EQ(c.nextStep(0.0), 0.0);
}

// y(t) = 1 - exp(-t) при a=0, b=1
TEST(Model37, EulerVsAnalytic) {
    Model37 m(0.0, 1.0, 0.001);
    double y = 0.0;
    for (int i = 0; i < 1000; ++i) y = m.nextStep(1.0);
    EXPECT_NEAR(y, 1.0 - std::exp(-1.0), 1e-3);
}

// сходимость всех моделей (u = 1) 
struct Case {
    std::function<std::unique_ptr<Model>()> make;
    double expected;
};
class Convergence : public ::testing::TestWithParam<Case> {};

TEST_P(Convergence, ReachesSteadyState) {
    std::unique_ptr<Model> m = GetParam().make();
    double y = 0.0;
    for (int i = 0; i < 2000; ++i) y = m->nextStep(1.0);
    EXPECT_NEAR(y, GetParam().expected, 1e-6);
}

INSTANTIATE_TEST_SUITE_P(AllModels, Convergence, ::testing::Values(
    Case{[] { return std::make_unique<Model19>(0.5, 0.2, 0.1, 1.0); }, 5.0},
    Case{[] { return std::make_unique<Model19>(0.3, 0.1, 0.1, 2.0); }, 4.0},
    Case{[] { return std::make_unique<Model23>(0.5, 1.0, 0.0); },      2.0},
    Case{[] { return std::make_unique<Model23>(0.5, 1.0, 0.2); },      1.6},
    Case{[] { return std::make_unique<Model37>(0.0, 1.0, 0.1); },      1.0},
    Case{[] { return std::make_unique<Model37>(0.5, 2.0, 0.1); },      2.0 / std::exp(0.5)}));
