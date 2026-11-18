#include <iostream>
#include <fstream>
#include <iomanip>
#include <memory>
#include <string>
#include <cstdlib> // Для std::system

#include "DynamicModel.h"
#include "SignalGenerator.h"
#include "ModelCreator.h"

namespace {
    constexpr int    DEFAULT_STEPS = 20;
    constexpr double DEFAULT_LEVEL = 1.0;
    constexpr int    CSV_PRECISION = 5;
}

// Простой ввод с проверкой
static int readInt(const std::string& prompt, int minValue)
{
    int value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value && value >= minValue)
        {
            return value;
        }
        std::cout << "  Invalid input. Try again.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

// Выбор типа сигнала
static std::unique_ptr<SignalGenerator> selectSignal(int choice)
{
    switch (choice)
    {
    case 1:
        return std::make_unique<StepSignal>(DEFAULT_LEVEL);
    case 2:
        return std::make_unique<BurstSignal>();
    case 3:
        return std::make_unique<HarmonicSignal>();
    default:
        return nullptr;
    }
}

int main()
{
    std::cout << " OTIS-2026 | Lab #1 | Variant 28\n";
    std::cout << " Student: ii02918\n";

    // 1. Выбор модели
    std::cout << "Select a model:\n";
    std::cout << "  1) Model 1.5 (State Delay)\n";
    std::cout << "  2) Model 2.2 (Saturation)\n";
    std::cout << "  3) Model 3.8 (Quadratic Decay ODE)\n";
    const int modelChoice = readInt("Your choice (1-3): ", 1);
    if (modelChoice > 3)
    {
        std::cerr << "Invalid model number.\n";
        return 1;
    }

    // 2. Создание модели
    std::cout << "\nEnter model parameters:\n";
    std::unique_ptr<DynamicModel> model = ModelCreator::create(modelChoice);
    if (!model)
    {
        std::cerr << "Failed to create model.\n";
        return 1;
    }
    std::cout << "\nModel: " << model->describe() << "\n";

    // 3. Проверка устойчивости
    if (!model->verifyStability())
    {
        if (const std::string warn = model->getWarningMessage(); !warn.empty())
        {
            std::cout << "\n!!! WARNING !!!\n" << warn << "\n";
        }
        std::cout << "Continue simulation? (y/n): ";
        std::string answer;
        std::cin >> answer;
        if (answer != "y" && answer != "Y")
        {
            std::cout << "Cancelled by user.\n";
            return 0;
        }
    }

    // 4. Выбор сигнала
    std::cout << "\nSelect input signal:\n";
    std::cout << "  1) Step (constant)\n";
    std::cout << "  2) Burst (impulse at tau=1)\n";
    std::cout << "  3) Harmonic (sin)\n";
    const int signalChoice = readInt("Your choice (1-3): ", 1);

    std::unique_ptr<SignalGenerator> signal = selectSignal(signalChoice);
    if (!signal)
    {
        std::cerr << "Invalid signal number.\n";
        return 1;
    }
    std::cout << "Signal: " << signal->getName() << "\n";

    // 5. Количество шагов
    const int steps = readInt("\nNumber of simulation steps n: ", 1);

    // 6. Симуляция
    std::cout << "\n============================================\n";
    std::cout << std::setw(6) << "tau"
        << std::setw(14) << "u_tau"
        << std::setw(14) << "y_tau" << "\n";
    std::cout << "--------------------------------------------\n";

    std::ofstream csv("result.csv");
    csv << "tau,u_tau,y_tau\n";

    model->reset();
    for (int tau = 1; tau <= steps; ++tau)
    {
        const double u = signal->value(tau);
        const double y = model->advance(u);

        std::cout << std::setw(6) << tau
            << std::setw(14) << std::fixed << std::setprecision(CSV_PRECISION) << u
            << std::setw(14) << std::fixed << std::setprecision(CSV_PRECISION) << y
            << "\n";

        csv << tau << "," << u << "," << y << "\n";
    }
    csv.close();

    std::cout << "--------------------------------------------\n";
    std::cout << "Results saved to result.csv\n";
    const std::string plotCmd = "python plot_results.py result.csv result_plot.png \"Simulation Result\"";
    if (const int result = std::system(plotCmd.c_str()); result == 0) 
    {
        const std::string openCmd = "start result_plot.png";
        std::system(openCmd.c_str());
    }
    else
    {
        std::cout << "[WARNING] Python not found or plot_results.py failed. Skipping plot.\n";
    }

    std::cout << "Done.\n";

    return 0;
}