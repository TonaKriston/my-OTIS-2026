#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>
#endif

#include "plot_config.h"

class PlotWriteError final : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class InputEndedError final : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// InputSignal supplies u[tau] for any simulation step.
class InputSignal {
public:
    virtual ~InputSignal() = default;
    virtual double valueAt(std::size_t tau) const = 0;
};

class ConstantSignal final : public InputSignal {
public:
    explicit ConstantSignal(double value) : value_(value) {}
    double valueAt(std::size_t) const override { return value_; }

private:
    double value_;
};

class ImpulseSignal final : public InputSignal {
public:
    double valueAt(std::size_t tau) const override { return tau == 0 ? 1.0 : 0.0; }
};

class HarmonicSignal final : public InputSignal {
public:
    double valueAt(std::size_t tau) const override {
        return std::sin(static_cast<double>(tau));
    }
};

// Before nextValue(tau, ...), histories contain values from index 0 through tau.
// In particular, inputHistory already includes u[tau].
class Model {
public:
    virtual ~Model() = default;
    virtual double nextValue(std::size_t tau,
                             const std::vector<double>& outputHistory,
                             const std::vector<double>& inputHistory) const = 0;
};

// Model 1.2: y[tau+1] = a*y[tau] + b*u[tau-k], with k >= 1.
// Input values before index 0 are defined as zero.
class DelayedInputModel final : public Model {
public:
    DelayedInputModel(double a, double b, std::size_t delay)
        : a_(a), b_(b), delay_(delay) {
        if (delay_ == 0) {
            throw std::invalid_argument("Delay k must be at least 1.");
        }
    }

    double nextValue(std::size_t tau,
                     const std::vector<double>& outputHistory,
                     const std::vector<double>& inputHistory) const override {
        const std::size_t delayedIndex = tau >= delay_ ? tau - delay_ : inputHistory.size();
        const double delayedInput = delayedIndex < inputHistory.size()
                                        ? inputHistory[delayedIndex]
                                        : 0.0;
        return a_ * outputHistory.at(tau) + b_ * delayedInput;
    }

private:
    double a_;
    double b_;
    std::size_t delay_;
};

// Model 2.7: y[tau+1] = a*y[tau] + b*relay(u[tau], y[tau]).
class RelayModel final : public Model {
public:
    RelayModel(double a, double b, double epsilon)
        : a_(a), b_(b), epsilon_(epsilon) {
        if (epsilon_ < 0.0) {
            throw std::invalid_argument("Epsilon must be non-negative.");
        }
    }

    double nextValue(std::size_t tau,
                     const std::vector<double>& outputHistory,
                     const std::vector<double>& inputHistory) const override {
        const double y = outputHistory.at(tau);
        const double u = inputHistory.at(tau);
        const double relay = (u > epsilon_ || (u >= -epsilon_ && y > 0.0))
                                 ? 1.0
                                 : -1.0;
        return a_ * y + b_ * relay;
    }

private:
    double a_;
    double b_;
    double epsilon_;
};

// Model 3.5: dy/dt = b*sin(u), integrated with an explicit Euler step.
class HarmonicDifferentialModel final : public Model {
public:
    HarmonicDifferentialModel(double b, double timeStep)
        : b_(b), timeStep_(timeStep) {
        if (!(timeStep_ > 0.0)) {
            throw std::invalid_argument("Time step dt must be positive.");
        }
    }

    double nextValue(std::size_t tau,
                     const std::vector<double>& outputHistory,
                     const std::vector<double>& inputHistory) const override {
        return outputHistory.at(tau) + timeStep_ * b_ * std::sin(inputHistory.at(tau));
    }

private:
    double b_;
    double timeStep_;
};

struct SimulationRow {
    std::size_t step;
    double input;
    double currentOutput;
    double nextOutput;
};

std::vector<SimulationRow> simulate(const Model& model,
                                    const InputSignal& signal,
                                    double initialOutput,
                                    std::size_t numberOfSteps) {
    if (numberOfSteps == 0) {
        throw std::invalid_argument("The number of simulation steps n must be positive.");
    }

    std::vector<double> inputHistory;
    std::vector outputHistory{initialOutput};
    std::vector<SimulationRow> rows;
    inputHistory.reserve(numberOfSteps);
    outputHistory.reserve(numberOfSteps + 1);
    rows.reserve(numberOfSteps);

    for (std::size_t tau = 0; tau < numberOfSteps; ++tau) {
        const double input = signal.valueAt(tau);
        inputHistory.push_back(input);

        const double currentOutput = outputHistory.at(tau);
        const double nextOutput = model.nextValue(tau, outputHistory, inputHistory);
        rows.push_back({tau + 1, input, currentOutput, nextOutput});
        outputHistory.push_back(nextOutput);
    }

    return rows;
}

void printTable(const std::vector<SimulationRow>& rows) {
    std::cout << std::fixed << std::setprecision(6);
    std::cout << std::setw(8) << "Step"
              << std::setw(16) << "Input u"
              << std::setw(16) << "Before y"
              << std::setw(16) << "After y" << '\n';
    std::cout << std::string(56, '-') << '\n';

    for (const SimulationRow& row : rows) {
        std::cout << std::setw(8) << row.step
                  << std::setw(16) << row.input
                  << std::setw(16) << row.currentOutput
                  << std::setw(16) << row.nextOutput << '\n';
    }
}

std::filesystem::path writeSvgPlot(const std::vector<SimulationRow>& rows) {
    if (rows.empty()) {
        throw std::invalid_argument("Cannot plot an empty simulation.");
    }

    std::vector<double> outputs;
    outputs.reserve(rows.size());
    for (const SimulationRow& row : rows) {
        outputs.push_back(row.nextOutput);
    }

    const auto [minimumIterator, maximumIterator] =
        std::minmax_element(outputs.begin(), outputs.end());
    double minimum = *minimumIterator;
    double maximum = *maximumIterator;
    double padding = (maximum - minimum) * 0.1;
    if (padding == 0.0) {
        padding = std::max(1.0, std::abs(maximum) * 0.1);
    }
    minimum -= padding;
    maximum += padding;

    constexpr double left = 76.0;
    constexpr double top = 56.0;
    constexpr double plotWidth = 700.0;
    constexpr double plotHeight = 390.0;
    const auto xCoordinate = [&](std::size_t index) {
        if (rows.size() == 1) {
            return left + plotWidth / 2.0;
        }
        return left + static_cast<double>(index) /
                          static_cast<double>(rows.size() - 1) * plotWidth;
    };
    const auto yCoordinate = [&](double value) {
        return top + (maximum - value) / (maximum - minimum) * plotHeight;
    };

    const auto timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
    const auto outputPath =
        std::filesystem::path(II03010_PLOT_DIRECTORY) /
        ("ii03010_task01_plot_" + std::to_string(timestamp) + ".svg");
    std::ofstream svg(outputPath);
    if (!svg) {
        throw PlotWriteError("Could not create the SVG plot in the build directory.");
    }

    svg << std::fixed << std::setprecision(2);
    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"860\" height=\"520\" viewBox=\"0 0 860 520\">\n";
    svg << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
    svg << "<text x=\"430\" y=\"28\" text-anchor=\"middle\" font-family=\"sans-serif\" font-size=\"20\">Output after each step</text>\n";

    for (int tick = 0; tick <= 4; ++tick) {
        const double ratio = static_cast<double>(tick) / 4.0;
        const double y = top + ratio * plotHeight;
        const double value = maximum - ratio * (maximum - minimum);
        svg << "<line x1=\"" << left << "\" y1=\"" << y << "\" x2=\"" << left + plotWidth
            << "\" y2=\"" << y << "\" stroke=\"#d9dee7\"/>\n";
        svg << "<text x=\"" << left - 10.0 << "\" y=\"" << y + 5.0
            << R"(" text-anchor="end" font-family="sans-serif" font-size="12">)"
            << value << "</text>\n";
    }

    svg << "<line x1=\"" << left << "\" y1=\"" << top << "\" x2=\"" << left
        << "\" y2=\"" << top + plotHeight << "\" stroke=\"#263238\"/>\n";
    svg << "<line x1=\"" << left << "\" y1=\"" << top + plotHeight << "\" x2=\""
        << left + plotWidth << "\" y2=\"" << top + plotHeight << "\" stroke=\"#263238\"/>\n";
    svg << "<text x=\"426\" y=\"500\" text-anchor=\"middle\" font-family=\"sans-serif\" font-size=\"14\">Step</text>\n";
    svg << "<text x=\"20\" y=\"250\" transform=\"rotate(-90 20 250)\" text-anchor=\"middle\" font-family=\"sans-serif\" font-size=\"14\">Output y</text>\n";

    std::ostringstream points;
    points << std::fixed << std::setprecision(2);
    for (std::size_t step = 0; step < outputs.size(); ++step) {
        points << xCoordinate(step) << ',' << yCoordinate(outputs[step]) << ' ';
    }
    svg << "<polyline points=\"" << points.str()
        << "\" fill=\"none\" stroke=\"#1769aa\" stroke-width=\"3\"/>\n";
    for (std::size_t step = 0; step < outputs.size(); ++step) {
        svg << "<circle cx=\"" << xCoordinate(step) << "\" cy=\""
            << yCoordinate(outputs[step]) << "\" r=\"4\" fill=\"#1769aa\"/>\n";
        if (step % std::max<std::size_t>(1, (rows.size() - 1) / 4) == 0 ||
            step + 1 == rows.size()) {
            svg << "<text x=\"" << xCoordinate(step) << "\" y=\""
                << top + plotHeight + 20.0
                << R"(" text-anchor="middle" font-family="sans-serif" font-size="12">)"
                << step + 1 << "</text>\n";
        }
    }
    svg << "</svg>\n";
    if (!svg) {
        throw PlotWriteError("Failed while writing the SVG plot.");
    }
    return outputPath;
}

bool openSvgPlot(const std::filesystem::path& plotPath) {
#ifdef _WIN32
    const HINSTANCE result = ShellExecuteW(
        nullptr, L"open", plotPath.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    return reinterpret_cast<INT_PTR>(result) > 32;
#else
    (void)plotPath;
    return false;
#endif
}

long long readInteger(const std::string& prompt) {
    long long value{};
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        if (std::cin.eof()) {
            throw InputEndedError("Input ended before all values were entered.");
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please enter a whole number.\n";
    }
}

long long readChoice(const std::string& prompt, long long minimum, long long maximum) {
    while (true) {
        if (const auto value = readInteger(prompt); value >= minimum && value <= maximum) {
            return value;
        }
        std::cout << "Choose a number from " << minimum << " to " << maximum << ".\n";
    }
}

std::size_t readPositiveSize(const std::string& prompt) {
    while (true) {
        if (const auto value = readInteger(prompt); value > 0) {
            return static_cast<std::size_t>(value);
        }
        std::cout << "The value must be a positive integer.\n";
    }
}

double readFiniteDouble(const std::string& prompt) {
    while (true) {
        double value{};
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (std::isfinite(value)) {
                return value;
            }
            std::cout << "Please enter a finite number.\n";
            continue;
        }
        if (std::cin.eof()) {
            throw InputEndedError("Input ended before all values were entered.");
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please enter a number (use a dot for decimals).\n";
    }
}

double readNonNegativeDouble(const std::string& prompt) {
    while (true) {
        if (const auto value = readFiniteDouble(prompt); value >= 0.0) {
            return value;
        }
        std::cout << "The value must be non-negative.\n";
    }
}

double readPositiveDouble(const std::string& prompt) {
    while (true) {
        if (const auto value = readFiniteDouble(prompt); value > 0.0) {
            return value;
        }
        std::cout << "The value must be positive.\n";
    }
}

int main() {
    try {
        std::cout << "OTIS - Laboratory Work 1\n";
        std::cout << "Student: ii03010\n";
        std::cout << "Variant: 21\n\n";

        std::cout << "Models:\n"
                  << "1 - Model 1.2 (delayed input)\n"
                  << "2 - Model 2.7 (relay with hysteresis)\n"
                  << "3 - Model 3.5 (differential equation)\n";
        const long long modelChoice = readChoice("Choose a model (1-3): ", 1, 3);

        std::cout << "\nInput signals:\n"
                  << "1 - Constant input\n"
                  << "2 - Impulse input (1 at step 1, then 0)\n"
                  << "3 - Harmonic input (sin(tau))\n";
        const long long signalChoice = readChoice("Choose an input signal (1-3): ", 1, 3);

        const std::size_t numberOfSteps = readPositiveSize("Number of simulation steps n: ");
        const double initialOutput = readFiniteDouble("Initial output y[0]: ");

        std::unique_ptr<Model> model;
        if (modelChoice == 1) {
            const double a = readFiniteDouble("Coefficient a: ");
            const double b = readFiniteDouble("Coefficient b: ");
            const std::size_t delay = readPositiveSize("Input delay k (k >= 1): ");
            model = std::make_unique<DelayedInputModel>(a, b, delay);
        } else if (modelChoice == 2) {
            const double a = readFiniteDouble("Coefficient a: ");
            const double b = readFiniteDouble("Coefficient b: ");
            const double epsilon = readNonNegativeDouble("Relay threshold epsilon (epsilon >= 0): ");
            model = std::make_unique<RelayModel>(a, b, epsilon);
        } else {
            const double b = readFiniteDouble("Coefficient b: ");
            const double timeStep = readPositiveDouble("Time step dt (dt > 0): ");
            model = std::make_unique<HarmonicDifferentialModel>(b, timeStep);
        }

        std::unique_ptr<InputSignal> signal;
        if (signalChoice == 1) {
            signal = std::make_unique<ConstantSignal>(readFiniteDouble("Constant input value: "));
        } else if (signalChoice == 2) {
            signal = std::make_unique<ImpulseSignal>();
        } else {
            signal = std::make_unique<HarmonicSignal>();
        }

        const std::vector<SimulationRow> rows =
            simulate(*model, *signal, initialOutput, numberOfSteps);
        std::cout << "\nSimulation results:\n";
        printTable(rows);
        const std::filesystem::path plotPath = writeSvgPlot(rows);
        std::cout << "\nSVG plot saved to: " << plotPath.string() << '\n';
        if (!openSvgPlot(plotPath)) {
            std::cerr << "Could not open the SVG plot automatically. "
                         "Open the saved file manually.\n";
        }
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
