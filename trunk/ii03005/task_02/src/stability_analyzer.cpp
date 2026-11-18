#include "stability_analyzer.h"

#include <algorithm>
#include <cmath>
#include <complex>
#include <stdexcept>


std::complex<double> rootDifferenceProduct(
    const std::vector<std::complex<double>>& roots,
    std::size_t rootIndex)
{
    std::complex<double> denominator{1.0, 0.0};
    for (std::size_t i = 0; i < roots.size(); ++i)
    {
        if (rootIndex != i)
        {
            denominator *= roots[rootIndex] - roots[i];
        }
    }
    return denominator;
}


std::vector<std::complex<double>> StabilityAnalyzer::roots(double a1, double a2, std::size_t delay){
    if (delay == 0)
    {
        throw std::invalid_argument("Delay must be at least one.");
    }

    const std::size_t degree = delay + 1;
    std::vector<std::complex<double>> roots(degree);
    constexpr double pi = 3.1415;
    for (std::size_t i = 0; i < degree; ++i)
    {
        const double angle = 2.0 * pi * static_cast<double>(i) / static_cast<double>(degree);
        roots[i] = std::polar(0.5, angle);
    }

    for (int _ = 0; _ < 2000; ++_)
    {
        double largestChange = 0.0;
        for (std::size_t i = 0; i < degree; ++i)
        {
            std::complex<double> value = std::pow(roots[i], degree) - a1 * std::pow(roots[i], delay) - a2;

            const std::complex<double> denominator = rootDifferenceProduct(roots, i);

            const std::complex<double> change = value / denominator;
            roots[i] -= change;

            largestChange = std::max(largestChange, std::abs(change));
        }
        if (largestChange < 1e-12)
        {
            break;
        }
    }

    return roots; // GCOVR_EXCL_LINE
} // GCOVR_EXCL_LINE

std::vector<double> StabilityAnalyzer::rootModuli(double a1, double a2, std::size_t delay)
{
    std::vector<double> moduli;
    for (const auto& root : roots(a1, a2, delay))
    {
        moduli.push_back(std::abs(root));
    }
    return moduli;
}

bool StabilityAnalyzer::isStable(double a1, double a2, std::size_t delay,  double tolerance)
{
    return std::ranges::all_of(
        rootModuli(a1, a2, delay),
        [tolerance](double modulus)
        {
            return modulus < 1.0 - tolerance;
        });
}
