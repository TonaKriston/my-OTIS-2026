#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <memory>
#include "InputSignal.h"
#include "Model.h"
#include "Model1_1.h"
#include "Model2_5.h"
#include "Model3_9.h"

// Безопасное ограничение количества шагов симуляции
int getSafeSteps(int raw_steps) {
    constexpr int min_steps = 1;
    constexpr int max_steps = 1000;
    constexpr int default_steps = 30;

    if (raw_steps < min_steps || raw_steps > max_steps) {
        return default_steps;
    }
    return raw_steps;
}

// Вспомогательная функция для ввода коэффициентов со значениями по умолчанию
double getDoubleInput(const std::string& prompt, double default_value) {
    std::cout << prompt << " (default " << default_value << "): ";
    if (std::string input; std::getline(std::cin, input) && !input.empty()) {
        std::stringstream ss(input);
        double val = 0.0;
        if (ss >> val) {
            return val;
        }
    }
    return default_value;
}

// Выбор модели
int getModelChoice() {
    std::cout << "Select model:\n"
              << "  1. Model 1.1 (Standard First-Order Inertial System: y[t+1] = a*y[t] + b*u[t])\n"
              << "  2. Model 2.5 (Signum Friction: y[t+1] = a*y[t] + b*sign(u)*(1 - exp(-|u|)))\n"
              << "  3. Model 3.9 (Bounded Saturation Rate: dy/dt = b*tanh(u))\n"
              << "Enter choice (1-3, default 1): ";
    if (std::string input; std::getline(std::cin, input) && !input.empty()) {
        std::stringstream ss(input);
        int val = 1;
        if (ss >> val && (val >= 1 && val <= 3)) {
            return val;
        }
    }
    return 1;
}

// Выбор входного сигнала
SignalType getSignalChoice() {
    std::cout << "\nSelect input signal:\n"
              << "  1. Step (u = A)\n"
              << "  2. Impulse (u(0) = A, u(t>0) = 0)\n"
              << "  3. Harmonic (u = A * sin(t))\n"
              << "Enter choice (1-3, default 1): ";
    if (std::string input; std::getline(std::cin, input) && !input.empty()) {
        std::stringstream ss(input);
        int val = 1;
        if (ss >> val) {
            switch (val) {
                case 2:
                    return SignalType::IMPULSE;
                case 3:
                    return SignalType::HARMONIC;
                case 1:
                default:
                    return SignalType::STEP;
            }
        }
    }
    return SignalType::STEP;
}

int main(int argc, char* argv[]) {
    int parsed_steps = 30;

    if (argc > 1) {
        try {
            parsed_steps = getSafeSteps(std::stoi(argv[1]));
        } catch (...) {
            parsed_steps = 30;
        }
    } else {
        std::cout << "Enter simulation steps (default 30): ";
        if (std::string input; std::getline(std::cin, input) && !input.empty()) {
            std::stringstream ss(input);
            int val = 0;
            if (ss >> val) {
                parsed_steps = getSafeSteps(val);
            }
        }
    }

    const int steps = parsed_steps;
    const int model_choice = getModelChoice();
    const SignalType selected_signal = getSignalChoice();

    std::unique_ptr<Model> active_model;
    std::string model_name;

    std::cout << "\n--- Parameters configuration ---\n";

    switch (model_choice) {
        case 2: {
            const double a = getDoubleInput("Enter parameter a", 0.7);
            const double b = getDoubleInput("Enter parameter b", 0.6);
            const double y0 = getDoubleInput("Enter initial y0", 0.0);
            active_model = std::make_unique<Model2_5>(a, b, y0);
            model_name = "Model 2.5";
            break;
        }
        case 3: {
            const double b = getDoubleInput("Enter parameter b", 0.8);
            const double dt = getDoubleInput("Enter time step dt", 0.2);
            const double y0 = getDoubleInput("Enter initial y0", 0.0);
            active_model = std::make_unique<Model3_9>(b, dt, y0);
            model_name = "Model 3.9";
            break;
        }
        case 1:
        default: {
            const double a = getDoubleInput("Enter parameter a", 0.8);
            const double b = getDoubleInput("Enter parameter b", 0.5);
            const double y0 = getDoubleInput("Enter initial y0", 0.0);
            active_model = std::make_unique<Model1_1>(a, b, y0);
            model_name = "Model 1.1";
            break;
        }
    }

    const std::string sig_name = InputSignal::getSignalName(selected_signal);

    std::ofstream out("results.csv");
    if (!out.is_open()) {
        std::cerr << "Failed to open results.csv\n";
        return 1;
    }

    out << "step,signal,u,y,model_name\n";

    std::cout << "\n============================================\n";
    std::cout << "Simulation started for selected configuration\n";
    std::cout << "============================================\n";

    std::cout << std::setw(6) << "Step"
              << std::setw(12) << "Input u"
              << std::setw(16) << "Output y" << "\n";

    for (int t = 0; t < steps; ++t) {
        const double u = InputSignal::getSignal(selected_signal, t);
        const double y = active_model->step(u);

        std::cout << std::setw(6) << t
                  << std::fixed << std::setprecision(4)
                  << std::setw(12) << u
                  << std::setw(16) << y << "\n";

        out << t << "," << sig_name << "," << u << ","
            << y << "," << model_name << "\n";
    }

    out.close();
    std::cout << "\nResults saved to results.csv\n";
    return 0;
}