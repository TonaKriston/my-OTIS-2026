#pragma once

#include <cstddef>
#include <complex>
#include <vector>

class StabilityAnalyzer
{
public:
    static std::vector<std::complex<double>> roots(double a1, double a2,
                                                   std::size_t delay);
    static bool isStable(double a1, double a2, std::size_t delay,
                         double tolerance = 1e-9);
    static std::vector<double> rootModuli(double a1, double a2,
                                          std::size_t delay);
};
