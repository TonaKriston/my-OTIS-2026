#include "models.hpp"
#include "signals.hpp"
#include "simulator.hpp"

#include <filesystem>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

template <typename T>
T readNumber(const std::string& prompt, T minValue, T maxValue) {
    T value{};
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minValue && value <= maxValue) {
            return value;
        }
        std::cout << "Invalid value. Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int readChoice(const std::string& prompt, int minValue, int maxValue) {
    return readNumber<int>(prompt, minValue, maxValue);
}

struct ModelParameters {
    double a{};
    double b{};
    double dt{};
    int delay{};
};

ModelParameters askParameters(int modelChoice) {
    ModelParameters p;
    std::cout << "\nEnter model coefficients.\n";
    if (modelChoice == 1) {
        std::cout << "Model 1.2: y[t+1] = a*y[t] + b*u[t-k]\n";
        p.a = readNumber<double>("a: ", -1000000.0, 1000000.0);
        p.b = readNumber<double>("b: ", -1000000.0, 1000000.0);
        p.delay = readNumber<int>("Control delay k (>= 1): ", 1, 1000000);
    } else if (modelChoice == 2) {
        std::cout << "Model 2.4: y[t+1] = a*y[t]*cos(y[t]) + b*ln(1+|u[t]|)\n";
        p.a = readNumber<double>("a: ", -1000000.0, 1000000.0);
        p.b = readNumber<double>("b: ", -1000000.0, 1000000.0);
    } else {
        std::cout << "Model 3.6: dy/dt = a*y^3 + b*u (Euler method)\n";
        p.a = readNumber<double>("a: ", -1000000.0, 1000000.0);
        p.b = readNumber<double>("b: ", -1000000.0, 1000000.0);
        p.dt = readNumber<double>("Time step dt (> 0): ", 0.000001, 1000000.0);
    }
    return p;
}

std::unique_ptr<otis::Model> createModel(int choice, const ModelParameters& p) {
    if (choice == 1) return std::make_unique<otis::DelayedControlModel>(p.a, p.b, p.delay);
    if (choice == 2) return std::make_unique<otis::LogarithmicInteractionModel>(p.a, p.b);
    return std::make_unique<otis::CubicDifferentialModel>(p.a, p.b, p.dt);
}

std::string modelFolder(int choice) {
    if (choice == 1) return "linear";
    if (choice == 2) return "nonlinear";
    return "differential";
}

const char* modelLabel(int choice) {
    if (choice == 1) return "Linear model 1.2";
    if (choice == 2) return "Nonlinear model 2.4";
    return "Differential model 3.6";
}

std::vector<int> choicesFromMenu(int choice) {
    if (choice == 4) return {1, 2, 3};
    return {choice};
}

std::vector<otis::SignalType> signalsFromMenu(int choice) {
    using otis::SignalType;
    if (choice == 4) return {SignalType::Step, SignalType::Impulse, SignalType::Harmonic};
    if (choice == 1) return {SignalType::Step};
    if (choice == 2) return {SignalType::Impulse};
    return {SignalType::Harmonic};
}

void clearPreviousResults(const fs::path& resultsDirectory) {
    // Keep the results directory, but remove all data and plots from earlier runs.
    if (fs::exists(resultsDirectory)) {
        fs::remove_all(resultsDirectory);
    }
    fs::create_directories(resultsDirectory);
}

int parseStepCount(const char* argument) {
    try {
        return std::stoi(argument);
    } catch (const std::invalid_argument&) {
        throw std::invalid_argument("The command-line step count must be an integer.");
    } catch (const std::out_of_range&) {
        throw std::invalid_argument("The command-line step count is outside the supported range.");
    }
}

int launchPlotScript(const fs::path& executableDirectory, const fs::path& resultsDirectory) {
    const fs::path script = executableDirectory / "plot.py";
    if (!fs::exists(script)) {
        std::cerr << "Warning: plot.py was not found: " << script.string() << '\n';
        return -1;
    }
#ifdef _WIN32
    const std::string command = "python \"" + script.string() + "\" \"" + resultsDirectory.string() + "\"";
#else
    const std::string command = "python3 \"" + script.string() + "\" \"" + resultsDirectory.string() + "\"";
#endif
    std::cout << "\nLaunching plot.py automatically...\n" << std::flush;
    return std::system(command.c_str());
}

} // namespace

int main(int argc, char* argv[]) {
    try {
        std::cout << "============================================\n"
                  << " OTIS Lab 1 - Variant 2: Interactive simulator\n"
                  << "============================================\n\n";

        std::cout << "Choose a model:\n"
                  << "  1. Linear model 1.2 (control delay)\n"
                  << "  2. Nonlinear model 2.4 (cosine + logarithm)\n"
                  << "  3. Differential model 3.6 (cubic growth, Euler method)\n"
                  << "  4. Run all three models\n";
        const int modelChoice = readChoice("Model [1-4]: ", 1, 4);

        std::cout << "\nChoose the input influence u(tau):\n"
                  << "  1. Step:     u(tau) = 1\n"
                  << "  2. Impulse:  u(0) = 1, u(tau > 0) = 0\n"
                  << "  3. Harmonic: u(tau) = sin(tau)\n"
                  << "  4. Run all three input signals\n";
        const int signalChoice = readChoice("Input signal [1-4]: ", 1, 4);

        int steps = 30;
        if (argc > 1) {
            steps = parseStepCount(argv[1]);
            if (steps < 1 || steps > 1000000)
                throw std::invalid_argument("The step count must be in range 1..1000000.");
            std::cout << "\nNumber of simulation steps n (from command line): " << steps << '\n';
        } else {
            steps = readNumber<int>("\nNumber of simulation steps n [1..1000000]: ", 1, 1000000);
        }

        const fs::path executableDirectory = fs::absolute(argv[0]).parent_path();
        const fs::path resultsDirectory = executableDirectory / "results";
        clearPreviousResults(resultsDirectory);
        fs::create_directories(resultsDirectory / "linear");
        fs::create_directories(resultsDirectory / "nonlinear");
        fs::create_directories(resultsDirectory / "differential");

        const auto selectedModels = choicesFromMenu(modelChoice);
        const auto selectedSignals = signalsFromMenu(signalChoice);

        for (const int selectedModel : selectedModels) {
            std::cout << "\n--------------------------------------------\n"
                      << "Selected: " << modelLabel(selectedModel) << '\n';
            const ModelParameters parameters = askParameters(selectedModel);
            auto model = createModel(selectedModel, parameters);
            const fs::path modelResults = resultsDirectory / modelFolder(selectedModel);
            for (const auto signal : selectedSignals) {
                otis::simulate(*model, steps, signal, modelResults);
            }
        }

        std::cout << "\nSimulation completed.\n"
                  << "Only results from this run are kept in: " << resultsDirectory.string() << '\n';
        if (const int plotStatus = launchPlotScript(executableDirectory, resultsDirectory);
            plotStatus != 0) {
            std::cerr << "\nAutomatic plotting failed. Check that Python and matplotlib are installed.\n"
                      << "Install matplotlib with: python -m pip install matplotlib\n";
            return 2;
        }
        std::cout << "\nAll tasks completed.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "\nError: " << error.what() << '\n';
        return 1;
    }
}
