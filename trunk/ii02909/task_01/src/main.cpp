#include <iostream>
#include <fstream>
#include <iomanip>
#include <memory>
#include <string>
#include <string_view>
#include <cctype>
#include <algorithm>

#include "Model.h"
#include "Model1_4.h"
#include "Model2_9.h"
#include "Model3_1.h"
#include "InputSignals.h"

namespace {
    // Значения по умолчанию для интерактивного ввода
    constexpr double DEFAULT_A = 0.9;
    constexpr double DEFAULT_B = 1.0;
    constexpr double DEFAULT_DT = 0.01;
    constexpr int    DEFAULT_N = 20;
    constexpr int    CSV_PRECISION = 5;
}

// ---------- Проверка, что строка состоит только из цифр ----------
static bool isAllDigits(std::string_view s) {
    return !s.empty() && std::all_of(s.begin(), s.end(),
        [](unsigned char c) { return std::isdigit(c) != 0; });
}

// ---------- Ввод числа с проверкой ----------
static double readDouble(std::string_view prompt, double def) {
    std::cout << prompt << " [default " << def << "]: ";
    std::string line;
    std::getline(std::cin, line);
    if (line.empty()) {
        return def;
    }
    try {
        return std::stod(line);
    }
    catch (const std::invalid_argument&) {
        std::cout << "  Not a number, using default value.\n";
        return def;
    }
    catch (const std::out_of_range&) {
        std::cout << "  Number out of range, using default value.\n";
        return def;
    }
}

static int readInt(std::string_view prompt, int def) {
    std::cout << prompt << " [default " << def << "]: ";
    std::string line;
    std::getline(std::cin, line);
    if (line.empty()) {
        return def;
    }
    try {
        return std::stoi(line);
    }
    catch (const std::invalid_argument&) {
        std::cout << "  Not a number, using default value.\n";
        return def;
    }
    catch (const std::out_of_range&) {
        std::cout << "  Number out of range, using default value.\n";
        return def;
    }
}

static int readChoice(std::string_view prompt, int lo, int hi) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);

        if (isAllDigits(line)) {
            try {
                const int v = std::stoi(line);
                if (v >= lo && v <= hi) {
                    return v;
                }
            }
            catch (const std::out_of_range&) {
                std::cout << "  Number out of range.\n";
            }
        }
        std::cout << "  Enter a number between " << lo << " and " << hi << ".\n";
    }
}

int main() {
    std::cout << "===========================================\n";
    std::cout << " Lab #1 (OTIS) - ii02909\n";
    std::cout << " Modeling of a controlled object\n";
    std::cout << "===========================================\n\n";

    // ---------- Выбор модели ----------
    std::cout << "Choose a model:\n";
    std::cout << "  1) Model 1.4 - Linear (Discrete Integrator with Loss)\n";
    std::cout << "  2) Model 2.9 - Non-linear (Square Root Modulated Action)\n";
    std::cout << "  3) Model 3.1 - Diff. equation (Pure Linear Decay)\n";
    const int modelChoice = readChoice("Your choice (1-3): ", 1, 3);
    std::cout << "\n";

    // ---------- Коэффициенты ----------
    const double a = readDouble("Enter coefficient a", DEFAULT_A);
    const double b = readDouble("Enter coefficient b", DEFAULT_B);
    double dt = DEFAULT_DT;
    if (modelChoice == 3) {
        dt = readDouble("Enter time step dt", DEFAULT_DT);
    }

    // ---------- Создание модели ----------
    std::unique_ptr<Model> model;
    switch (modelChoice) {
        case 1:
            model = std::make_unique<Model1_4>(a, b);
            break;
        case 2:
            model = std::make_unique<Model2_9>(a, b);
            break;
        case 3:
            model = std::make_unique<Model3_1>(a, dt);
            break;
        default:
            std::cerr << "Unknown model choice\n";
            return 1;
    }

    std::cout << "\nModel: " << model->name() << "\n";

    // ---------- Проверка устойчивости ----------
    if (!model->isStable()) {
        if (const std::string warn = model->stabilityWarning(); !warn.empty()) {
            std::cout << "\n!!! WARNING !!!\n" << warn << "\n";
        }
        std::cout << "Continue simulation? (y/n): ";
        std::string ans;
        std::getline(std::cin, ans);
        if (!(ans == "y" || ans == "Y")) {
            std::cout << "Cancelled by user.\n";
            return 0;
        }
    }

    // ---------- Выбор входного сигнала ----------
    std::cout << "\nChoose input signal:\n";
    std::cout << "  1) Step\n";
    std::cout << "  2) Impulse\n";
    std::cout << "  3) Harmonic\n";
    const int sigChoice = readChoice("Your choice (1-3): ", 1, 3);

    std::unique_ptr<InputSignal> signal;
    switch (sigChoice) {
        case 1:
            signal = std::make_unique<StepInput>();
            break;
        case 2:
            signal = std::make_unique<ImpulseInput>();
            break;
        case 3:
            signal = std::make_unique<HarmonicInput>();
            break;
        default:
            std::cerr << "Unknown signal choice\n";
            return 1;
    }
    std::cout << "Signal: " << signal->name() << "\n";

    // ---------- Кол-во шагов ----------
    const int n = readInt("\nEnter number of simulation steps n", DEFAULT_N);
    if (n <= 0) {
        std::cout << "n must be > 0\n";
        return 1;
    }

    // ---------- Симуляция ----------
    std::cout << "\n===========================================\n";
    std::cout << std::setw(6) << "tau"
        << std::setw(14) << "u_tau"
        << std::setw(14) << "y_tau" << "\n";
    std::cout << "-------------------------------------------\n";

    std::ofstream csv("result.csv");
    csv << "tau,u_tau,y_tau\n";

    model->reset();
    for (int tau = 1; tau <= n; ++tau) {
        const double u = signal->value(tau);
        const double y = model->next(u);

        std::cout << std::setw(6) << tau
            << std::setw(14) << std::fixed << std::setprecision(CSV_PRECISION) << u
            << std::setw(14) << std::fixed << std::setprecision(CSV_PRECISION) << y
            << "\n";

        csv << tau << "," << u << "," << y << "\n";
    }

    csv.close();
    std::cout << "-------------------------------------------\n";
    std::cout << "Results saved to result.csv\n";
    std::cout << "Done.\n";

    return 0;
}