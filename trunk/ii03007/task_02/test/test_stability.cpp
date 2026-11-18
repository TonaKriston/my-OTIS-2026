#include <gtest/gtest.h>
#include <array>
#include <sstream>
#include <string>

#include "stability.h"

struct CaseStab {
    const char* name;
    double a1, a2, a3;
    bool expectedStable;
};

class Stability : public ::testing::TestWithParam<CaseStab> {};

TEST_P(Stability, VerdictMatchesExpectation) {
    const CaseStab c = GetParam();
    EXPECT_EQ(stable16(c.a1, c.a2, c.a3), c.expectedStable)
        << "набор: " << c.name;
}

static const std::array<CaseStab, 7> kSets{{
    {"a0_3_a2_0_1",       0.3,  0.2, 0.1,  true},
    {"a0_5_a2_0_05",      0.5,  0.3, 0.05, true},
    {"a0_5_am0_3_a0_05",  0.5, -0.3, 0.05, true},
    {"a0_2_a2_0_05",      0.2,  0.3, 0.05, true},
    {"a1_2_a2_0_1",       1.2,  0.3, 0.1,  false},
    {"a0_2_a2_0_9",       0.2,  0.9, 0.1,  false},
    {"boundary_z1",       0.0,  1.0, 0.0,  false},
}};

INSTANTIATE_TEST_SUITE_P(Verdicts, Stability, ::testing::ValuesIn(kSets),
    [](const ::testing::TestParamInfo<CaseStab>& info) {
        return info.param.name;
    });


TEST(StabilityValues, RootsOfStableSet) {
    // z^3 - 0.3z^2 - 0.2z - 0.1 = 0:
    // z1 = 0.746971, z2,3 = -0.223485 +/- j0.289704, |z2| = 0.365888
    CubicRoots r = cubicRoots(0.3, 0.2, 0.1);
    EXPECT_NEAR(r.re[0], 0.746971, 1e-5);
    EXPECT_NEAR(r.re[1], -0.223485, 1e-5);
    EXPECT_NEAR(std::fabs(r.im[1]), 0.289704, 1e-5);
    EXPECT_NEAR(r.mod[0], 0.746971, 1e-5);
    EXPECT_NEAR(r.mod[1], 0.365888, 1e-5);
    EXPECT_NEAR(r.mod[2], r.mod[1], 1e-12);    
}

TEST(StabilityValues, RootsOfUnstableSet) {
    // z^3 - 1.2z^2 - 0.3z - 0.1 = 0 -> z1 = 1.453692
    CubicRoots r = cubicRoots(1.2, 0.3, 0.1);
    EXPECT_NEAR(r.re[0], 1.453692, 1e-5);
    EXPECT_FALSE(r.stable);
}


TEST(StabilityValues, ZeroCoefficientsGiveTripleZeroRoot) {
    // z^3 = 0: тройной корень в нуле, система устойчива
    CubicRoots r = cubicRoots(0.0, 0.0, 0.0);
    for (int k = 0; k < 3; ++k) {
        EXPECT_NEAR(r.re[k], 0.0, 1e-12);
        EXPECT_NEAR(r.mod[k], 0.0, 1e-12);
    }
    EXPECT_TRUE(r.stable);
}


TEST(StabilityBoundary, PlusOneOnTheCircle) {
    // z^3 - z = z(z-1)(z+1): корни 1, 0, -1
    CubicRoots r = cubicRoots(0.0, 1.0, 0.0);
    EXPECT_NEAR(r.mod[0], 1.0, 1e-9);
    EXPECT_NEAR(r.mod[1], 0.0, 1e-9);
    EXPECT_NEAR(r.mod[2], 1.0, 1e-9);
    EXPECT_FALSE(stable16(0.0, 1.0, 0.0))   
        << "корень на окружности не должен считаться устойчивым";
}

TEST(StabilityBoundary, SimpleRootOneOnTheCircle) {
    // z^3 - 0.5z^2 - 0.5z = z(z-1)(z+0.5): корни 1, 0, -0.5
    CubicRoots r = cubicRoots(0.5, 0.5, 0.0);
    EXPECT_NEAR(r.re[0], 1.0, 1e-12);
    EXPECT_NEAR(r.mod[2], 0.5, 1e-9);
    EXPECT_FALSE(stable16(0.5, 0.5, 0.0));
}


TEST(StabilityReport, WarnsWhenUnstable) {
    std::ostringstream out;
    printStabilityCheck(1.2, 0.3, 0.1, out);
    EXPECT_NE(out.str().find("WARNING"), std::string::npos)
        << "в отчёте о неустойчивости должно быть предупреждение:\n" << out.str();
    EXPECT_NE(out.str().find("1.4537"), std::string::npos)
        << "корень вне круга должен напечататься:\n" << out.str();
}

TEST(StabilityReport, NoWarningWhenStable) {
    std::ostringstream out;
    printStabilityCheck(0.3, 0.2, 0.1, out);
    EXPECT_EQ(out.str().find("WARNING"), std::string::npos);
    EXPECT_NE(out.str().find("STABLE"), std::string::npos)
        << "в устойчивом отчёте должен быть вердикт:\n" << out.str();
}