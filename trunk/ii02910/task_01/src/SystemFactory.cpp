#include "SystemFactory.h"
#include "LinearARXModel.h"
#include "SaturationModel.h"
#include "CubicODE.h"

#include <iostream>
#include <limits>
#include <stdexcept>

namespace
{
    double readNumber(const std::string& prompt)
    {
        while (true)
        {
            std::cout << prompt;
            if (double value; std::cin >> value && std::isfinite(value))
            {
                return value;
            }
            std::cout << "Invalid input. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::unique_ptr<AbstractSystem> SystemFactory::create(ModelType type)
{
    switch (type)
    {
    case ModelType::LinearARX:
    {
        std::cout << "\n--- Model 1.8 parameters ---\n";
        double a1 = readNumber("a1 = ");
        double a2 = readNumber("a2 = ");
        double b1 = readNumber("b1 = ");
        double b2 = readNumber("b2 = ");
        return std::make_unique<LinearARXModel>(a1, a2, b1, b2);
    }
    case ModelType::Saturation:
    {
        std::cout << "\n--- Model 2.2 parameters ---\n";
        double a = readNumber("a  = ");
        double b = readNumber("b  = ");
        double uMin = readNumber("uMin = ");
        double uMax = readNumber("uMax = ");
        return std::make_unique<SaturationModel>(a, b, uMin, uMax);
    }
    case ModelType::CubicODE:
    {
        std::cout << "\n--- Model 3.6 parameters ---\n";
        double a = readNumber("a  = ");
        double b = readNumber("b  = ");
        double dt = readNumber("dt = ");
        return std::make_unique<CubicODE>(a, b, dt);
    }
    default:
        throw std::invalid_argument("Unknown model type");
    }
}