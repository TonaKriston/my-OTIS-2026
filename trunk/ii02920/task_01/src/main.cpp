#include "Model.h"
#include "Model1_10.h"
#include "Model2_5.h"
#include "Model3_10.h"
#include "InputSignals.h"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

class SimulationError final : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// Прототипы функций.
int readInt(const std::string& prompt, int defaultValue, int minimum, int maximum);
double readDouble(const std::string& prompt, double defaultValue);
bool simulate(Model& model, const InputSignal& signal, int n);
void writeRow(std::ofstream& csv, int tau, double u, double y);

int main() {
    try {
        std::cout << "===========================================\n"
                  << " Lab #1 (OTIS) - ii02920 - variant 25\n"
                  << " Modeling of a controlled object\n"
                  << "===========================================\n"
                  << "Press Enter to use the default value.\n\n"
                  << "Choose a model:\n"
                  << "  1) Model 1.10 - Linear, two-step input delay\n"
                  << "  2) Model 2.5  - Non-linear input\n"
                  << "  3) Model 3.10 - Differential equation, Euler method\n";

        int choice = readInt("Your choice (1-3)", 1, 1, 3);
        double a = readDouble(choice == 1 ? "Enter a1" : "Enter a",
                              choice == 3 ? 1.0 : 0.5);
        double b = readDouble(choice == 1 ? "Enter b1" : "Enter b",
                              choice == 3 ? 0.5 : 0.4);
        double b2 = 0.0;
        double dt = 0.1;
        if (choice == 1) {
            b2 = readDouble("Enter b2", 0.1);
        }
        if (choice == 3) {
            do {
                dt = readDouble("Enter positive dt", 0.1);
                if (dt <= 0.0) {
                    std::cout << "dt must be positive.\n";
                }
            } while (dt <= 0.0);
        }
        double y0 = readDouble("Enter initial value y0", 0.0);

        std::unique_ptr<Model> model;
        switch (choice) {
        case 1:
            model = std::make_unique<Model1_10>(a, b, b2, y0);
            break;
        case 2:
            model = std::make_unique<Model2_5>(a, b, y0);
            break;
        case 3:
            model = std::make_unique<Model3_10>(a, b, dt, y0);
            break;
        default:
            throw SimulationError("Unknown model selection.");
        }

        std::cout << "\nModel: " << model->name() << '\n';
        if (!model->isStable()) {
            std::cout << model->stabilityWarning() << '\n'
                      << "Continue simulation? (y/n): ";
            std::string answer;
            if (!std::getline(std::cin, answer)) {
                throw SimulationError("Input ended before all parameters were entered.");
            }
            if (answer != "y" && answer != "Y") {
                std::cout << "Cancelled by user.\n";
                return 0;
            }
        }

        std::cout << "\nChoose input signal:\n"
                  << "  1) Step: u(tau) = A\n"
                  << "  2) Impulse: u(0) = 1, u(tau > 0) = 0\n"
                  << "  3) Harmonic: u(tau) = sin(tau)\n";
        int signalChoice = readInt("Your choice (1-3)", 1, 1, 3);
        std::unique_ptr<InputSignal> signal;
        switch (signalChoice) {
        case 1: {
            double amplitude = readDouble("Enter step amplitude A", 1.0);
            signal = std::make_unique<StepInput>(amplitude);
            break;
        }
        case 2:
            signal = std::make_unique<ImpulseInput>();
            break;
        case 3:
            signal = std::make_unique<HarmonicInput>();
            break;
        default:
            throw SimulationError("Unknown input signal selection.");
        }

        int n = readInt("Enter number of simulation steps n", 20, 1, 10000);
        if (choice == 3 && !std::isfinite(n * dt)) {
            throw SimulationError("Final time is too large. Reduce dt or n.");
        }
        std::cout << "Signal: " << signal->name() << '\n';
        if (!simulate(*model, *signal, n)) {
            return 1;
        }

        std::cout << "Results saved to result.csv in the current working directory.\n"
                  << model->name() << "\"\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "ERROR: " << error.what() << '\n';
        return 1;
    }
}

// Реализация функций.
int readInt(const std::string& prompt, int defaultValue, int minimum, int maximum) {
    while (true) {
        std::cout << prompt << " [default " << defaultValue << "]: ";
        std::string line;
        if (!std::getline(std::cin, line)) {
            throw SimulationError("Input ended before all parameters were entered.");
        }
        if (line.find_first_not_of(" \t\r") == std::string::npos) {
            return defaultValue;
        }
        std::istringstream input(line);
        int value = 0;
        if (char extra = 0; (input >> value) && !(input >> extra) &&
            value >= minimum && value <= maximum) {
            return value;
        }
        std::cout << "Enter an integer from " << minimum << " to " << maximum << ".\n";
    }
}

double readDouble(const std::string& prompt, double defaultValue) {
    while (true) {
        std::cout << prompt << " [default " << defaultValue << "]: ";
        std::string line;
        if (!std::getline(std::cin, line)) {
            throw SimulationError("Input ended before all parameters were entered.");
        }
        if (line.find_first_not_of(" \t\r") == std::string::npos) {
            return defaultValue;
        }
        std::istringstream input(line);
        double value = 0.0;
        if (char extra = 0; (input >> value) && !(input >> extra) && std::isfinite(value)) {
            return value;
        }
        std::cout << "Enter a finite number; use a dot as the decimal separator.\n";
    }
}

bool simulate(Model& model, const InputSignal& signal, int n) {
    std::ofstream csv("result.csv");
    if (!csv) {
        std::cerr << "ERROR: cannot create result.csv.\n";
        return false;
    }
    csv << "tau,u_tau,y_tau\n" << std::setprecision(17);
    std::cout << '\n' << std::setw(6) << "tau"
              << std::setw(18) << "u_tau" << std::setw(18) << "y_tau" << '\n'
              << "------------------------------------------\n"
              << std::setprecision(8);

    model.reset();
    for (int tau = 0; tau <= n; tau++) {
        double u = signal.value(tau);
        writeRow(csv, tau, u, model.value());
        if (tau < n && !std::isfinite(model.next(u))) {
            std::cerr << "ERROR: numerical overflow while calculating y(" << tau + 1
                      << "). The CSV contains only the completed rows.\n";
            return false;
        }
    }
    csv.close();
    if (!csv) {
        std::cerr << "ERROR: failed to write result.csv.\n";
        return false;
    }
    std::cout << "------------------------------------------\n";
    return true;
}

void writeRow(std::ofstream& csv, int tau, double u, double y) {
    csv << tau << ',' << u << ',' << y << '\n';
    std::cout << std::setw(6) << tau << std::setw(18) << u
              << std::setw(18) << y << '\n';
}
