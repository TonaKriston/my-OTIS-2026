#include "simulator.hpp"
#include <array>
#include <cmath>
#include <ios>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

namespace otis {
namespace fs = std::filesystem;

void simulate(Model& model, int steps, SignalType signal, const fs::path& outputDirectory) {
    const fs::path csvPath = outputDirectory / (std::string(signalName(signal)) + ".csv");
    std::ofstream csv(csvPath);
    if (!csv) throw std::ios_base::failure("Could not create file: " + csvPath.string());
    csv << "tau,u,y\n" << std::fixed << std::setprecision(8);

    std::cout << "\n" << model.name() << " | signal: " << signalName(signal) << '\n';
    std::cout << std::left << std::setw(8) << "tau" << std::setw(16) << "u(tau)" << "y(tau+1)\n";
    std::cout << std::string(40, '-') << '\n';
    model.reset();
    for (int tau = 0; tau < steps; ++tau) {
        const double input = inputSignal(signal, tau);
        const double output = model.step(input, tau);
        if (!std::isfinite(output)) {
            std::cout << "Warning: non-finite value at tau=" << tau << "; simulation stopped.\n";
            break;
        }
        csv << tau << ',' << input << ',' << output << '\n';
        std::cout << std::left << std::setw(8) << tau << std::setw(16)
                  << std::setprecision(6) << input << std::setprecision(8) << output << '\n';
    }
    if (!csv) throw std::ios_base::failure("Error writing file: " + csvPath.string());
    std::cout << "CSV saved: " << csvPath.string() << '\n';
}

void generatePlotScript(const fs::path& outDir) {
    struct Plot { const char* folder; const char* title; const char* png; };
    const std::array<Plot, 9> plots{{
        {"linear", "Linear model 1.2 - step input", "linear_step.png"},
        {"linear", "Linear model 1.2 - impulse input", "linear_impulse.png"},
        {"linear", "Linear model 1.2 - harmonic input", "linear_harmonic.png"},
        {"nonlinear", "Nonlinear model 2.4 - step input", "nonlinear_step.png"},
        {"nonlinear", "Nonlinear model 2.4 - impulse input", "nonlinear_impulse.png"},
        {"nonlinear", "Nonlinear model 2.4 - harmonic input", "nonlinear_harmonic.png"},
        {"differential", "Differential model 3.6 - step input", "differential_step.png"},
        {"differential", "Differential model 3.6 - impulse input", "differential_impulse.png"},
        {"differential", "Differential model 3.6 - harmonic input", "differential_harmonic.png"}
    }};
    std::ofstream gp(outDir / "plot_all.gp");
    if (!gp) throw std::ios_base::failure("Could not create plot_all.gp");
    gp << "set terminal pngcairo size 1000,650\nset grid\nset xlabel 'tau'\nset ylabel 'y'\n";
    bool hasPlots = false;
    for (const auto& plot : plots) {
        const std::string pngName(plot.png);
        const char* signal = "harmonic";
        if (pngName.find("step") != std::string::npos) {
            signal = "step";
        } else if (pngName.find("impulse") != std::string::npos) {
            signal = "impulse";
        }
        if (const fs::path csvPath = outDir / plot.folder / (std::string(signal) + ".csv");
            !fs::exists(csvPath)) {
            continue;
        }
        hasPlots = true;
        gp << "set output '" << plot.png << "'\nset title '" << plot.title << "'\n"
           << "plot '" << plot.folder << "/" << signal
           << ".csv' using 1:3 every ::1 with lines lw 2 title 'y'\n";
    }
    if (!hasPlots) gp << "# Run a simulation first; no CSV files were found.\n";
    std::cout << "\nPlot script written to " << (outDir / "plot_all.gp")
              << ". From the results directory run: gnuplot plot_all.gp\n";
}

} // namespace otis
