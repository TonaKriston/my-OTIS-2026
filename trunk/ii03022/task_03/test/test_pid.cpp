#include <gtest/gtest.h>
#include <array>
#include <cmath>

#include "PIDController.h"
#include "../../task_01/src/Model 1.8.h"

TEST(PIDMath, FirstStepIsDirectTerm){
    PIDController pid(2.0, 0.5, 0.2, 1.0, -10.0, 10.0);
    // e=1, история нулевая: dU = 2*1 + 0.5*1 + 0.2*1 = 2.7
    EXPECT_DOUBLE_EQ(pid.step(1.0, 0.0), 2.7);
}

TEST(PIDMath, SecondStepUsesErrorHistory){
    PIDController pid(2.0, 0.5, 0.2, 1.0, -10.0, 10.0);
    pid.step(1.0, 0.0);   
    EXPECT_NEAR(pid.step(1.0, 0.5), 2.7 - 1.05, 1e-12);
}

TEST(PIDClamp, RespectsActuatorLimits){
    PIDController pid(100.0, 0.0, 0.0, 1.0, -5.0, 5.0);
    EXPECT_DOUBLE_EQ(pid.step(50.0, 0.0), 5.0);   // 5000 обрезано до uMax
    EXPECT_DOUBLE_EQ(pid.step(50.0, 0.0), 5.0);   // дальше держится: dU = 0
}

TEST(PIDReset, RestoresInitialState){
    PIDController pid(2.0, 0.5, 0.2, 1.0, -10.0, 10.0);
    pid.step(1.0, 0.0);
    pid.reset();
    EXPECT_DOUBLE_EQ(pid.step(1.0, 0.0), 2.7);    // как у свежего регулятора
}

// замкнутый контур: объект 1.8 (0.5; 0.3; 1; 0.5), w = 10, ПИД (0.3; 0.1; 0.1)
TEST(PIDClosedLoop, FollowsReferenceTrajectory){
    PIDController pid(0.3, 0.1, 0.1, 1.0, -100.0, 100.0);
    GeneralizedAutoregressiveLinearModel plant(0.5, 0.3, 1.0, 0.5);
    const std::array<double, 4> expectedU{{5.0, 2.5, 2.25, 2.125}};
    const std::array<double, 4> expectedY{{5.0, 7.5, 8.75, 9.875}};
    double y = 0;
    for (std::size_t t = 0; t < expectedY.size(); ++t){
        double u = pid.step(10.0, y);
        EXPECT_NEAR(u, expectedU[t], 1e-9) << "step " << t;
        y = plant.nextStep(u);
        EXPECT_NEAR(y, expectedY[t], 1e-9) << "step " << t;
    }
}

// тот же контур: за конечное время y выходит на w с точностью eps
TEST(PIDClosedLoop, ReachesSetpointWithinEps){
    PIDController pid(0.3, 0.1, 0.1, 1.0, -100.0, 100.0);
    GeneralizedAutoregressiveLinearModel plant(0.5, 0.3, 1.0, 0.5);
    const double w = 10.0;
    double y = 0;
    for (int t = 0; t < 25; t++)          // по факту хватает 18 шагов
        y = plant.nextStep(pid.step(w, y));
    EXPECT_NEAR(w - y, 0.0, 0.05);
}
