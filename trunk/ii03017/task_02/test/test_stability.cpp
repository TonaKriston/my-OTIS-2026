#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <string>
#include "StabilityAnalyzer.h"

TEST(StabilityAnalyzerTest, StableRealRootsInsideUnitCircle)
{
    // z^2 - 0.5z - 0.2 = 0 -> roots approx 0.7623 and -0.2623.
    const auto result = analyzeModel1_3Stability(0.5, 0.2);
    EXPECT_TRUE(result.stable);
    EXPECT_LT(std::hypot(result.root1Real, result.root1Imag), 1.0);
    EXPECT_LT(std::hypot(result.root2Real, result.root2Imag), 1.0);
}

TEST(StabilityAnalyzerTest, UnstableWhenRootOutsideUnitCircle)
{
    const auto result = analyzeModel1_3Stability(1.5, 0.0);
    EXPECT_FALSE(result.stable);
    EXPECT_GE(std::hypot(result.root1Real, result.root1Imag), 1.0);
}

TEST(StabilityAnalyzerTest, BoundaryRootOnUnitCircleIsNotAsymptoticallyStable)
{
    const auto result = analyzeModel1_3Stability(1.0, 0.0);
    EXPECT_FALSE(result.stable);
    EXPECT_NEAR(std::hypot(result.root1Real, result.root1Imag), 1.0, 1e-12);
}

TEST(StabilityAnalyzerTest, ComplexRootsCanBeStable)
{
    // a1=0, a2=-0.25 -> z^2 + 0.25=0; roots +/- 0.5i.
    const auto result = analyzeModel1_3Stability(0.0, -0.25);
    EXPECT_TRUE(result.stable);
    EXPECT_NEAR(std::abs(result.root1Imag), 0.5, 1e-12);
}

TEST(StabilityAnalyzerTest, NonFiniteCoefficientsAreRejected)
{
    const auto result = analyzeModel1_3Stability(std::numeric_limits<double>::infinity(), 0.0);
    EXPECT_FALSE(result.stable);
}


TEST(StabilityAnalyzerTest, NegativeDiscriminantProducesComplexConjugateRoots)
{
    const auto result = analyzeModel1_3Stability(0.0, -0.25);
    EXPECT_TRUE(result.stable);
    EXPECT_NEAR(result.root1Real, 0.0, 1e-12);
    EXPECT_NEAR(result.root2Real, 0.0, 1e-12);
    EXPECT_NEAR(result.root1Imag, -result.root2Imag, 1e-12);
}

TEST(StabilityAnalyzerTest, RejectsNonFiniteSecondCoefficient)
{
    const auto result = analyzeModel1_3Stability(0.0, std::numeric_limits<double>::quiet_NaN());
    EXPECT_FALSE(result.stable);
    ASSERT_NE(result.message, nullptr);
    EXPECT_STRNE(result.message, "");
}

TEST(StabilityAnalyzerTest, ReportsUnstableBoundaryRoot)
{
    const auto result = analyzeModel1_3Stability(-2.0, -1.0);
    EXPECT_FALSE(result.stable);
    ASSERT_NE(result.message, nullptr);
    EXPECT_STRNE(result.message, "");
}
