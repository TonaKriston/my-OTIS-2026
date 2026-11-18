#include "ApplicationLogic.h"
#include "StabilityAnalyzer.h"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <string>

bool requiresIntegrationStep(const char* modelName)
{
    return modelName != nullptr && std::string(modelName).find("Model 3.7") != std::string::npos;
}

bool isValidSignalChoice(int choice)
{
    return choice >= 1 && choice <= 3;
}

bool isValidModelChoice(int choice)
{
    return choice >= 1 && choice <= 3;
}

void printResultsTable(std::ostream& out, const std::vector<Row>& rows)
{
    out << "\n" << std::setw(8) << "tau" << std::setw(15) << "u"
        << std::setw(15) << "y" << '\n';
    out << std::string(38, '-') << '\n';
    for (const Row& row : rows)
        out << std::setw(8) << row.tau
            << std::setw(15) << std::fixed << std::setprecision(6) << row.u
            << std::setw(15) << row.y << '\n';
}

bool saveResultsCsv(const std::string& path, const std::vector<Row>& rows)
{
    std::ofstream file(path);
    if (!file)
        return false;
    file << "tau,u,y\n";
    for (const Row& row : rows)
        file << row.tau << ',' << row.u << ',' << row.y << '\n';
    return static_cast<bool>(file);
}

bool printStabilityReport(std::ostream& out, double a1, double a2)
{
    const StabilityResult result = analyzeModel1_3Stability(a1, a2);

    if (std::isfinite(result.root1Real))
    {
        out << "\nАнализ устойчивости (z^2 - a1*z - a2 = 0):\n"
            << "  z1 = " << result.root1Real << (result.root1Imag < 0.0 ? " - " : " + ")
            << std::abs(result.root1Imag) << "i\n"
            << "  z2 = " << result.root2Real << (result.root2Imag < 0.0 ? " - " : " + ")
            << std::abs(result.root2Imag) << "i\n";
    }
    out << "  " << result.message << '\n';
    return result.stable;
}
