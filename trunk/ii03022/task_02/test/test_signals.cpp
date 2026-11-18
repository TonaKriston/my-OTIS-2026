#include <gtest/gtest.h>
#include <cmath>

#include "InputSignal.h"

// ступеньчатые: на каждом такте одна и та же амплитуда
TEST(InputSignal, StepIsConstant){
    for (int t = 0; t < 5; t++)
        EXPECT_DOUBLE_EQ(inputSignal(t, 1, 2.5), 2.5) << "t=" << t;
}

// импульс: A только на первом такте, дальше чистый ноль
TEST(InputSignal, ImpulseOnlyAtStart){
    EXPECT_DOUBLE_EQ(inputSignal(0, 2, 3.0), 3.0);
    for (int t = 1; t < 5; t++)
        EXPECT_DOUBLE_EQ(inputSignal(t, 2, 3.0), 0.0) << "t=" << t;
}

// гармоничные: u(t) = A*sin(t)
TEST(InputSignal, HarmonicFollowsSine){
    double A = 2.0;
    EXPECT_DOUBLE_EQ(inputSignal(0, 3, A), 0.0);        // sin(0) = 0
    EXPECT_NEAR(inputSignal(1, 3, A), A * std::sin(1.0), 1e-12);
    EXPECT_NEAR(inputSignal(2, 3, A), A * std::sin(2.0), 1e-12);
}

// неизвестный тип сигнала не должен "зажигать" объект: возвращаем 0
TEST(InputSignal, UnknownTypeIsSilent){
    EXPECT_DOUBLE_EQ(inputSignal(0, 0, 5.0), 0.0);
    EXPECT_DOUBLE_EQ(inputSignal(3, 9, 5.0), 0.0);
}
