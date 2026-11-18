#include <gtest/gtest.h>
#include "stability.h"

// Модель 2.3
TEST(Stability23, StableValues)   { EXPECT_TRUE(isStable23(0.5)); EXPECT_TRUE(isStable23(-0.9)); }
TEST(Stability23, UnstableValues) { EXPECT_FALSE(isStable23(2.0)); EXPECT_FALSE(isStable23(-1.5)); }
TEST(Stability23, Boundary)       { EXPECT_FALSE(isStable23(1.0)); EXPECT_FALSE(isStable23(-1.0)); }

// Модель 3.7
TEST(Stability37, Stable)   { EXPECT_TRUE(isStable37(0.0, 0.1)); }
TEST(Stability37, Boundary) { EXPECT_FALSE(isStable37(0.0, 2.0)); }
TEST(Stability37, Unstable) { EXPECT_FALSE(isStable37(0.0, 3.0)); }

// Модель 1.9
TEST(Stability19, Stable)              { EXPECT_TRUE(isStable19(0.5, 0.2, 0.1)); }
TEST(Stability19, StableNegativeA1)    { EXPECT_TRUE(isStable19(-0.5, 0.0, 0.0)); }
TEST(Stability19, BoundaryZ1)          { EXPECT_FALSE(isStable19(1.0, 0.0, 0.0)); }
TEST(Stability19, BoundaryZMinus1)     { EXPECT_FALSE(isStable19(-1.0, 0.0, 0.0)); }
TEST(Stability19, UnstableA1)          { EXPECT_FALSE(isStable19(2.0, 0.0, 0.0)); }
TEST(Stability19, A3OnUnitCircle)      { EXPECT_FALSE(isStable19(0.0, 0.0, 1.0)); }
TEST(Stability19, ComplexRootsOutside) { EXPECT_FALSE(isStable19(0.0, -1.5, 0.0)); }