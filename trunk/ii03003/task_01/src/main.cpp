#include "models.hpp"
#include "simulation.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

namespace fs = std::filesystem;

struct Options {
    std::size_t steps = 40;
    double dt = 0.1;
    fs::path outDir = "results";
    double linearA = 0.7;
    double linearB = 0.5;
    std::size_t delay = 2;
    double nonlinearA1 = 0.25;
    double nonlinearA2 = 0.15;
    double nonlinearB = 0.6;
    double odeA = 0.7;
    double odeB = 0.3;
};

void printHelp() {
    std::cout << "Laboratory #1, variant 12: models 1.2, 2.6 and 3.10\n"
                 "Usage: lab1 [options]\n\n"
                 "  --steps N         Simulation steps; N > 0 (default: 40)\n"
                 "  --dt VALUE        Euler method time step > 0 (default: 0.1)\n"
                 "  --out PATH        Output folder (default: results)\n"
                 "  --linear-a VALUE  Model 1.2 coefficient a (default: 0.7)\n"
                 "  --linear-b VALUE  Model 1.2 coefficient b (default: 0.5)\n"
                 "  --delay K         Model 1.2 input delay K >= 1 (default: 2)\n"
                 "  --nonlinear-a1 V  Model 2.6 coefficient a1 (default: 0.25)\n"
                 "  --nonlinear-a2 V  Model 2.6 coefficient a2 (default: 0.15)\n"
                 "  --nonlinear-b V   Model 2.6 coefficient b (default: 0.6)\n"
                 "  --ode-a VALUE     Model 3.10 coefficient a (default: 0.7)\n"
                 "  --ode-b VALUE     Model 3.10 constant offset b (default: 0.3)\n"
                 "  --help            Show this help\n\n"
                 "Choose a model interactively. Only its 3 CSV tables and SVG graph are generated.\n";
}

double parseDouble(const std::string& value, const std::string& flag) {
    std::size_t end = 0;
    const double result = std::stod(value, &end);
    if (end != value.size() || !std::isfinite(result)) {
        throw std::invalid_argument("Invalid number for " + flag + ": " + value);
    }
    return result;
}

std::size_t parsePositiveInt(const std::string& value, const std::string& flag) {
    if (value.empty() || value[0] == '-') {
        throw std::invalid_argument("Invalid positive integer for " + flag);
    }
    std::size_t end = 0;
    const auto n = std::stoull(value, &end);
    if (end != value.size() || n == 0 || n > 100000) {
        throw std::invalid_argument("Expected an integer between 1 and 100000 for " + flag);
    }
    return static_cast<std::size_t>(n);
}

Options parseArgs(int argc, char* argv[]) {
    Options o;
    int i = 1;
    while (i < argc) {
        const std::string flag = argv[i];
        if (flag == "--help" || flag == "-h") {
            printHelp();
            std::exit(0);
        }
        if (i + 1 >= argc) throw std::invalid_argument("Missing value for " + flag);
        const std::string value = argv[i + 1];
        i += 2;
        if (flag == "--steps") o.steps = parsePositiveInt(value, flag);
        else if (flag == "--delay") o.delay = parsePositiveInt(value, flag);
        else if (flag == "--dt") o.dt = parseDouble(value, flag);
        else if (flag == "--out") o.outDir = value;
        else if (flag == "--linear-a") o.linearA = parseDouble(value, flag);
        else if (flag == "--linear-b") o.linearB = parseDouble(value, flag);
        else if (flag == "--nonlinear-a1") o.nonlinearA1 = parseDouble(value, flag);
        else if (flag == "--nonlinear-a2") o.nonlinearA2 = parseDouble(value, flag);
        else if (flag == "--nonlinear-b") o.nonlinearB = parseDouble(value, flag);
        else if (flag == "--ode-a") o.odeA = parseDouble(value, flag);
        else if (flag == "--ode-b") o.odeB = parseDouble(value, flag);
        else throw std::invalid_argument("Unknown option: " + flag);
    }
    if (!(o.dt > 0)) throw std::invalid_argument("--dt must be greater than zero");
    return o;
}

void printTable(const std::vector<Row>& rows, const std::string& signal) {
    std::cout << "\n  Input signal: " << signal << "\n"
              << "   tau       u_tau          y_tau       y_tau+1\n"
              << "  ------------------------------------------------\n";
    for (const Row& r : rows) {
        std::cout << std::fixed << std::setprecision(6)
                  << std::setw(6) << r.tau
                  << std::setw(12) << r.input
                  << std::setw(15) << r.yCurrent
                  << std::setw(15) << r.yNext << '\n';
    }
}

// On Windows, open the SVG in the default application right after calculating it.
// In headless environments, the chart remains available at the printed path.
void openChart(const fs::path& chart) {
    const fs::path absoluteChart = fs::absolute(chart);
    std::cout << "\nGraph saved: " << chart.generic_string() << '\n';
#ifdef _WIN32
    const HINSTANCE result = ShellExecuteW(nullptr, L"open", absoluteChart.c_str(),
                                           nullptr, nullptr, SW_SHOWNORMAL);
    if (reinterpret_cast<std::intptr_t>(result) <= 32) {
        std::cerr << "Could not open graph automatically; open the SVG path above.\n";
    }
#else
    std::cout << "Open the SVG file in your web browser to view the graph.\n";
#endif
}

void runSelectedModel(const Model& model, const Options& opts) {
    std::cout << "\n=== " << model.title() << " ===\n";
    if (model.id() == "model_1_2" && std::abs(opts.linearA) >= 1.0) {
        std::cout << "WARNING: Model 1.2 is not asymptotically stable (|a| >= 1).\n";
    }

    const auto constant = simulate(model, InputType::Step, opts.steps, opts.dt);
    const auto impulse = simulate(model, InputType::Impulse, opts.steps, opts.dt);
    const auto harmonic = simulate(model, InputType::Harmonic, opts.steps, opts.dt);

    // All three input types are displayed, but only for the selected model.
    for (const auto& [kind, rows] : {
             std::pair{InputType::Step, &constant},
             std::pair{InputType::Impulse, &impulse},
             std::pair{InputType::Harmonic, &harmonic}}) {
        printTable(*rows, inputName(kind));
        const fs::path csvPath = opts.outDir / (model.id() + "_" + inputName(kind) + ".csv");
        writeCsv(csvPath, *rows);
    }

    const fs::path svgPath = opts.outDir / (model.id() + ".svg");
    writeSvg(svgPath, model, constant, impulse, harmonic);
    std::cout << "\nDone: 3 CSV tables and 1 SVG chart for " << model.id() << ".\n";
    openChart(svgPath);
}

int main(int argc, char* argv[]) {
    try {
        const auto opts = parseArgs(argc, argv);
        fs::create_directories(opts.outDir);
        std::vector<std::unique_ptr<Model>> models;
        models.push_back(std::make_unique<DelayedLinearModel>(
            opts.linearA, opts.linearB, opts.delay));
        models.push_back(std::make_unique<PolynomialNonlinearModel>(
            opts.nonlinearA1, opts.nonlinearA2, opts.nonlinearB));
        models.push_back(std::make_unique<OffsetEulerModel>(opts.odeA, opts.odeB));

        std::cout << "Lab #1, variant 12 (ii03003); n=" << opts.steps
                  << ", dt=" << opts.dt << '\n';
        std::cout << "Initial conditions: y(-1)=0 and y(0)=0.\n";
        std::cout << "Available signals for each model: constant, impulse, sin(tau).\n";

        // Show the menu again after each calculation; 0 closes the application.
        bool menuActive = true;
        while (menuActive) {
            std::cout << "\nChoose which MODEL to calculate and display:\n"
                      << "  1 - Model 1.2 (linear input delay)\n"
                      << "  2 - Model 2.6 (nonlinear polynomial)\n"
                      << "  3 - Model 3.10 (differential, Euler method)\n"
                      << "  0 - Exit\n"
                      << "Your choice: " << std::flush;

            std::string choice;
            if (!std::getline(std::cin, choice)) {
                std::cout << "\nInput closed. Exiting.\n";
                menuActive = false;
                continue;
            }
            // Ignore whitespace around a typed menu number.
            const auto first = choice.find_first_not_of(" \t\r\n");
            if (first == std::string::npos) {
                std::cout << "Please enter 0, 1, 2, or 3.\n";
                continue;
            }
            const auto last = choice.find_last_not_of(" \t\r\n");
            choice = choice.substr(first, last - first + 1);
            if (choice == "0") {
                std::cout << "Goodbye!\n";
                menuActive = false;
                continue;
            }
            if (choice != "1" && choice != "2" && choice != "3") {
                std::cout << "Invalid choice: enter 0, 1, 2, or 3.\n";
                continue;
            }
            const auto index = static_cast<std::size_t>(choice[0] - '1');
            runSelectedModel(*models.at(index), opts);
        }
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        return 1;
    }
}
