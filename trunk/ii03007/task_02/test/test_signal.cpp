#include <gtest/gtest.h>
#include <cmath>

#include "signal.h"

TEST(Signal, StepIsConstant) {
    for (int t = 0; t < 5; ++t)
        EXPECT_DOUBLE_EQ(checkSignal(t, 1, 3.5), 3.5);
}

TEST(Signal, ImpulseOnlyAtFirstStep) {
    EXPECT_DOUBLE_EQ(checkSignal(0, 2, 2.0), 2.0);
    for (int t = 1; t < 5; ++t)
        EXPECT_DOUBLE_EQ(checkSignal(t, 2, 2.0), 0.0);
}

TEST(Signal, HarmonicMatchesSine) {
    for (int t = 0; t <= 10; ++t)
        EXPECT_NEAR(checkSignal(t, 3, 2.0), 2.0 * std::sin(t), 1e-12);
}

TEST(Signal, UnknownTypeIsSilent) {
    for (int t = 0; t < 3; ++t)
        EXPECT_DOUBLE_EQ(checkSignal(t, 9, 5.0), 0.0);
}