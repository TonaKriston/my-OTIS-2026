#include <gtest/gtest.h>
#include <cstdio>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include "ApplicationLogic.h"

TEST(ApplicationLogicTest, ValidatesSignalChoicesAtBothBoundaries)
{
    EXPECT_FALSE(isValidSignalChoice(0));
    EXPECT_TRUE(isValidSignalChoice(1));
    EXPECT_TRUE(isValidSignalChoice(2));
    EXPECT_TRUE(isValidSignalChoice(3));
    EXPECT_FALSE(isValidSignalChoice(4));
}

TEST(ApplicationLogicTest, ValidatesModelChoicesAtBothBoundaries)
{
    EXPECT_FALSE(isValidModelChoice(-1));
    EXPECT_FALSE(isValidModelChoice(0));
    EXPECT_TRUE(isValidModelChoice(1));
    EXPECT_TRUE(isValidModelChoice(2));
    EXPECT_TRUE(isValidModelChoice(3));
    EXPECT_FALSE(isValidModelChoice(4));
}

TEST(ApplicationLogicTest, IntegrationStepIsRequiredOnlyForModel37)
{
    EXPECT_TRUE(requiresIntegrationStep("Model 3.7"));
    EXPECT_FALSE(requiresIntegrationStep("Model 1.3"));
    EXPECT_FALSE(requiresIntegrationStep("Model 2.5"));
    EXPECT_FALSE(requiresIntegrationStep(nullptr));
}

TEST(ApplicationLogicTest, PrintsHeaderAndEveryRow)
{
    std::ostringstream out;
    printResultsTable(out, {{0, 1.0, 2.0}, {1, 3.0, 4.0}});
    const std::string text = out.str();
    EXPECT_NE(text.find("tau"), std::string::npos);
    EXPECT_NE(text.find("u"), std::string::npos);
    EXPECT_NE(text.find("y"), std::string::npos);
    EXPECT_NE(text.find("2.000000"), std::string::npos);
    EXPECT_NE(text.find("4.000000"), std::string::npos);
}

TEST(ApplicationLogicTest, SavesCsvAndReportsFailureForInvalidPath)
{
    const std::string path = "coverage_test_results.csv";
    ASSERT_TRUE(saveResultsCsv(path, {{0, 1.25, 2.5}, {1, 3.0, 4.0}}));
    std::ifstream file(path);
    ASSERT_TRUE(file.good());
    std::stringstream contents;
    contents << file.rdbuf();
    EXPECT_NE(contents.str().find("tau,u,y"), std::string::npos);
    EXPECT_NE(contents.str().find("0,1.25,2.5"), std::string::npos);
    file.close();
    std::remove(path.c_str());
    EXPECT_FALSE(saveResultsCsv("missing_directory/should_fail.csv", {}));
}

TEST(ApplicationLogicTest, StabilityReportForStableModelHasNoWarning)
{
    std::ostringstream out;
    EXPECT_TRUE(printStabilityReport(out, 0.5, 0.2));
    EXPECT_NE(out.str().find("устойчива"), std::string::npos);
    EXPECT_EQ(out.str().find("Предупреждение"), std::string::npos);
}

TEST(ApplicationLogicTest, StabilityReportForUnstableModelPrintsWarning)
{
    std::ostringstream out;
    EXPECT_FALSE(printStabilityReport(out, 1.5, 0.0));
    EXPECT_NE(out.str().find("Предупреждение"), std::string::npos);
}

TEST(ApplicationLogicTest, StabilityReportForBoundaryRootPrintsWarning)
{
    std::ostringstream out;
    EXPECT_FALSE(printStabilityReport(out, 1.0, 0.0));
    EXPECT_NE(out.str().find("Предупреждение"), std::string::npos);
}

TEST(ApplicationLogicTest, StabilityReportForNonFiniteCoefficientsSkipsRoots)
{
    std::ostringstream out;
    EXPECT_FALSE(printStabilityReport(out, std::numeric_limits<double>::infinity(), 0.0));
    EXPECT_EQ(out.str().find("z1"), std::string::npos);
    EXPECT_NE(out.str().find("конечными"), std::string::npos);
}
