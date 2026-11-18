#include <gtest/gtest.h>
#include "StabilityAnalyzer.h"

TEST(StabilityAnalyzerTest, StableSystem) {
    EXPECT_TRUE(StabilityAnalyzer::isStable(0.5, 0.2)); 
    EXPECT_TRUE(StabilityAnalyzer::isStable(1.0, -0.5));
}
TEST(StabilityAnalyzerTest, BoundaryValues) {
    EXPECT_FALSE(StabilityAnalyzer::isStable(2.0, 0.0)); 
    EXPECT_FALSE(StabilityAnalyzer::isStable(0.0, -1.0));
}
TEST(StabilityAnalyzerTest, UnstableSystem) {
    EXPECT_FALSE(StabilityAnalyzer::isStable(1.5, 0.0)); 
}
TEST(StabilityAnalyzerTest, RootsInfoGeneration) {
    EXPECT_NE(StabilityAnalyzer::getRootsInfo(2.0, 0.0).find("z1"), std::string::npos);
    EXPECT_NE(StabilityAnalyzer::getRootsInfo(0.0, -2.0).find("Комплексные"), std::string::npos);
}