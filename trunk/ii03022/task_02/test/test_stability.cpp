#include <gtest/gtest.h>
#include <array>
#include <sstream>
#include <string>
#include <cmath>

#include "StabilityAnalyzer.h"

struct CaseStab{
    double a1, a2;
    bool expectedStable;
};

static std::string NamePart(double v){
    std::ostringstream s;
    s << v;
    std::string r = s.str();
    for (char& c : r){
        if (c == '-') c = 'm';
        else if (c == '.') c = '_';
    }
    return r;
}

class StabilityTest : public ::testing::TestWithParam<CaseStab> {};

TEST_P(StabilityTest, ReturnsExpectedVerdict){
    CaseStab c = GetParam();
    EXPECT_EQ(isStable(c.a1, c.a2), c.expectedStable)
        << "a1=" << c.a1 << " a2=" << c.a2;
}

static const std::array<CaseStab, 8> kCasesStab{{
    {0.5,  0.3,  true},    // корни 0.8521 / -0.3521 -> в круге
    {0.9,  0.05, true},
    {0.5, -0.5,  true},    // комплексные, |z| = sqrt(0.5) = 0.7071
    {1.2,  0.3,  false},   // z1 = 1.4124 вне круга
    {0.5, -1.2,  false},
    {1.0,  0.0,  false},   // граница: z = 1 ровно на окружности
    {0.5,  0.5,  false},   // граница: z1 = 1.0
    {0.0, -1.0,  false},   // граница: |z| = 1 (комплексная пара)
}};
// [fix] 4-й аргумент - генератор имён: в отчёте ctest будут читаемые имена
// вида "c0_a0_5_b0_3_ok" вместо "24-byte object <...>"
INSTANTIATE_TEST_SUITE_P(Verdicts, StabilityTest, ::testing::ValuesIn(kCasesStab),
    [](const ::testing::TestParamInfo<CaseStab>& info){
        const CaseStab& p = info.param;
        return "c" + std::to_string(info.index) +
               "_a" + NamePart(p.a1) + "_b" + NamePart(p.a2) +
               (p.expectedStable ? "_ok" : "_bad");
    });

TEST(StabilityRoots, RealRootsValues){
    StabilityResult r = analyzeStability(0.5, 0.3);
    EXPECT_FALSE(r.isComplex);
    EXPECT_NEAR(r.mod1, 0.8520797, 1e-6);   // (0.5 + sqrt(1.45))/2
    EXPECT_NEAR(r.mod2, 0.3520797, 1e-6);
    EXPECT_TRUE(r.isStable);
}

TEST(StabilityRoots, ComplexPairModulus){
    StabilityResult r = analyzeStability(0.5, -0.5);
    EXPECT_TRUE(r.isComplex);
    // |z| = sqrt(re^2 + im^2) = sqrt(-a2) = sqrt(0.5)
    EXPECT_NEAR(r.mod1, std::sqrt(0.5), 1e-9);
    EXPECT_NEAR(r.z1re, 0.25, 1e-12);
    EXPECT_NEAR(r.z1im, 0.6614378, 1e-6);
}

TEST(StabilityReport, WarnsWhenUnstable){
    std::ostringstream out;
    printStabilityReport(1.2, 0.3, out);   // нестабильный набор
    std::string s = out.str();
    EXPECT_NE(s.find("UNSTABLE"), std::string::npos);
    EXPECT_NE(s.find("WARNING"),  std::string::npos);
}

TEST(StabilityReport, ConfirmsWhenStable){
    std::ostringstream out;
    printStabilityReport(0.5, 0.3, out);   // стабильный набор
    std::string s = out.str();
    EXPECT_NE(s.find("STABLE"),  std::string::npos);
    EXPECT_EQ(s.find("WARNING"), std::string::npos);   // предупреждения быть не должно
}

TEST(StabilityReport, PrintsComplexRootsBranch){
    std::ostringstream out;
    printStabilityReport(0.5, -0.5, out);   // D < 0 -> комплексная пара
    EXPECT_NE(out.str().find(" + j"), std::string::npos);
}
