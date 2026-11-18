#include "Model15.h"
#include "Model27.h"
#include "Model39.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

enum class InputType { Constant = 1, Impulse = 2, Harmonic = 3 };
class CsvFileException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

double inputValue(InputType type, int step, double amplitude) {
    switch (type) {
        case InputType::Constant: return amplitude;
        case InputType::Impulse: return step == 0 ? amplitude : 0.0;
        case InputType::Harmonic: return amplitude * std::sin(static_cast<double>(step));
    }
    return 0.0;
}

std::string inputName(InputType type) {
    switch (type) {
        case InputType::Constant: return "constant";
        case InputType::Impulse: return "impulse";
        case InputType::Harmonic: return "harmonic";
    }
    return "unknown";
}

std::vector<Sample> simulate(const Model& model, InputType type, double amplitude,
                             int steps, double dt) {
    std::vector<Sample> result;
    std::vector<double> history;
    result.reserve(static_cast<std::size_t>(steps) + 1);

    // Initial condition y(0)=0.
    history.push_back(0.0);
    result.push_back({0, 0.0, inputValue(type, 0, amplitude), 0.0});

    for (int step = 0; step < steps; ++step) {
        const double u = inputValue(type, step, amplitude);
        const double nextY = model.calculateNext(u, step, history, dt);
        history.push_back(nextY);
        result.push_back({step + 1, (step + 1) * dt,
                          inputValue(type, step + 1, amplitude), nextY});
    }
    return result;
}

void saveCsv(const std::string& filename, const std::vector<Sample>& samples) {
    std::ofstream out(filename);
    if (!out) throw CsvFileException("Cannot create file: " + filename);
    out << "step,tau,u,y\n" << std::setprecision(12);
    for (const auto& s : samples)
        out << s.step << ',' << s.time << ',' << s.u << ',' << s.y << '\n';
}

void printSamples(const std::vector<Sample>& samples) {
    std::cout << "step\t time\t\t u\t\t y\n";
    std::cout << std::fixed << std::setprecision(5);
    for (const auto& s : samples)
        std::cout << s.step << '\t' << s.time << "\t\t " << s.u << "\t\t " << s.y << '\n';
}

void runModel(const Model& model, int steps, double dt, double amplitude) {
    std::cout << "\n=== " << model.name() << " ===\n";
    for (int choice = 1; choice <= 3; ++choice) {
        const auto type = static_cast<InputType>(choice);
        const auto samples = simulate(model, type, amplitude, steps, dt);
        std::string modelNumber;
if (model.name() == "Model 1.5") {
    modelNumber = "15";
} else if (model.name() == "Model 2.7") {
    modelNumber = "27";
} else {
    modelNumber = "39";
}

const std::string filename =
    "model" + modelNumber + "_" + inputName(type) + ".csv";
        saveCsv(filename, samples);
        std::cout << "\nInput: " << inputName(type) << " | saved: " << filename << '\n';
        printSamples(samples);
    }
}

int main() {
    try {
        std::cout << "OTIS-2026 / Lab 1 / Variant 5\n";
        std::cout << "Demonstration parameters are used; change them in main.cpp if required.\n";
        const int steps = 40;
        const double dt = 0.1;
        const double amplitude = 1.0;

        // Illustrative coefficients: replace with values required by your instructor, if specified.
        Model15 model15(0.55, 0.20, 0.35, 2);
        Model27 model27(0.75, 0.40, 0.20);
        Model39 model39(0.80);

        runModel(model15, steps, dt, amplitude);
        runModel(model27, steps, dt, amplitude);
        runModel(model39, steps, dt, amplitude);

        std::cout << "\nDone. Nine CSV files were written to the working directory.\n";
        std::cout << "Press Enter to exit...";
        std::cin.get();
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }
    return 0;
}
