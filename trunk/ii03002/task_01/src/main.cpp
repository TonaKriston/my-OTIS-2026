#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <memory>
#include <algorithm>
#include "InputSignal.h"
#include "Model.h"
#include "Model1_9.h"
#include "Model2_1.h"
#include "Model3_3.h"

constexpr const char* EXPORT_FILENAME = "results.csv";

struct SimulationContext {
    int duration_steps{30};
    int selected_system{1};
    SignalType excitation_mode{SignalType::STEP};
};

static double requestDouble(const char* message, double fallback) {
    std::cout << message << " (default " << fallback << "): ";
    if (std::string line; std::getline(std::cin, line) && !line.empty()) {
        std::istringstream stream(line);
        double value = 0.0;
        if (stream >> value) {
            return value;
        }
    }
    return fallback;
}

static int requestInteger(const char* message, int fallback, int min_val, int max_val) {
    std::cout << message << " (default " << fallback << "): ";
    if (std::string line; std::getline(std::cin, line) && !line.empty()) {
        std::istringstream stream(line);
        int value = 0;
        if (stream >> value) {
            return std::clamp(value, min_val, max_val);
        }
    }
    return fallback;
}

static SimulationContext gatherUserPreferences(int argc, char* argv[]) {
    SimulationContext ctx;

    if (argc > 1) {
        std::istringstream arg_stream(argv[1]);
        int parsed_arg = 0;
        if (arg_stream >> parsed_arg) {
            ctx.duration_steps = std::clamp(parsed_arg, 1, 1000);
        }
    } else {
        ctx.duration_steps = requestInteger("Specify simulation duration", 30, 1, 1000);
    }

    std::cout << "\nAvailable structures:\n"
              << " [1] Model 1.9 (Three-Step Moving Average)\n"
              << " [2] Model 2.1 (Quadratic Feedback)\n"
              << " [3] Model 3.3 (Standard First-Order Process)\n";
    ctx.selected_system = requestInteger("Which structure?", 1, 1, 3);

    std::cout << "\nAvailable signals:\n"
              << " [1] Step function\n"
              << " [2] Dirac impulse\n"
              << " [3] Harmonic wave\n";
              
    // Оставлен только один вызов внутри условия if
    if (const int sig_choice = requestInteger("Which signal?", 1, 1, 3); sig_choice == 2) {
        ctx.excitation_mode = SignalType::IMPULSE;
    } else if (sig_choice == 3) {
        ctx.excitation_mode = SignalType::HARMONIC;
    } else {
        ctx.excitation_mode = SignalType::STEP;
    }

    return ctx; // Добавлен обязательный возврат структуры
}

static std::unique_ptr<Model> initializeSystem(const SimulationContext& ctx, std::string& label) {
    std::cout << "\n--- Parameter Setup ---\n";
    if (ctx.selected_system == 2) {
        label = "Model 2.1";
        const double a = requestDouble("Param a", 0.5);
        const double b = requestDouble("Param b", 0.1);
        const double c = requestDouble("Param c", 0.4);
        const double d = requestDouble("Param d", 0.2);
        const double y0 = requestDouble("Initial y0", 0.0);
        return std::make_unique<Model2_1>(a, b, c, d, y0);
    }
    if (ctx.selected_system == 3) {
        label = "Model 3.3";
        const double a = requestDouble("Param a", 0.5);
        const double b = requestDouble("Param b", 0.8);
        const double dt = requestDouble("Time step dt", 0.1);
        const double y0 = requestDouble("Initial y0", 0.0);
        return std::make_unique<Model3_3>(a, b, dt, y0);
    }
    
    label = "Model 1.9";
    const double a1 = requestDouble("Param a1", 0.4);
    const double a2 = requestDouble("Param a2", 0.3);
    const double a3 = requestDouble("Param a3", 0.1);
    const double b1 = requestDouble("Param b1", 0.5);
    const double y0 = requestDouble("Initial y0", 0.0);
    return std::make_unique<Model1_9>(a1, a2, a3, b1, y0);
}

int main(int argc, char* argv[]) {
    const SimulationContext config = gatherUserPreferences(argc, argv);
    
    std::string system_name;
    std::unique_ptr<Model> active_system = initializeSystem(config, system_name);

    std::ofstream log_file(EXPORT_FILENAME);
    if (!log_file.is_open()) {
        std::cerr << "Error: Could not create " << EXPORT_FILENAME << "\n";
        return 1;
    }

    log_file << "step,signal,u,y,model_name\n";
    const std::string waveform_name = InputSignal::getSignalName(config.excitation_mode);

    std::cout << "\n============================================\n";
    std::cout << " Process simulation initialized\n";
    std::cout << "============================================\n";
    std::cout << std::setw(6) << "Tick"
              << std::setw(12) << "Input (u)"
              << std::setw(16) << "State (y)" << "\n";

    for (int k = 0; k < config.duration_steps; ++k) {
        const double current_u = InputSignal::getSignal(config.excitation_mode, k);
        const double current_y = active_system->step(current_u);

        std::cout << std::setw(6) << k
                  << std::fixed << std::setprecision(4)
                  << std::setw(12) << current_u
                  << std::setw(16) << current_y << "\n";

        log_file << k << "," << waveform_name << "," << current_u << ","
                 << current_y << "," << system_name << "\n";
    }

    std::cout << "\nSuccessfully wrote data to " << EXPORT_FILENAME << "\n";
    return 0;
}