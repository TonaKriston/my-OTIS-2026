#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>
#include <array>
#include "Models.h"

double getInput(int type, int tau) {
    if (type == 0) return 1.0;
    if (type == 1) return (tau == 0) ? 1.0 : 0.0;
    if (type == 2) return std::sin(tau);
    return 0.0;
}

void simulateAndSave(model_standard* model, const std::string& filename, int n, int inputType, const char* modelName) {
    std::ofstream file(filename);
    file << "tau,u,y\n";

    std::cout << "\n=== Results: " << modelName << " ===\n";
    std::cout << std::fixed << std::setprecision(4);
    std::cout << std::setw(6) << "tau" << " | "
        << std::setw(10) << "u(tau)" << " | "
        << std::setw(10) << "y(tau)" << "\n";
    std::cout << "-------------------------------\n";

    model->reset();

    for (int tau = 0; tau <= n; ++tau) {
        double u = getInput(inputType, tau);
        double y = model->nextStep(u);

        file << tau << "," << u << "," << y << "\n";

        std::cout << std::setw(6) << tau << " | "
            << std::setw(10) << u << " | "
            << std::setw(10) << y << "\n";
    }
    file.close();
}

int main() {
    int n;
    std::cout << "Enter number of simulation steps (n): ";
    std::cin >> n;

    double a1;
    double b1_1;
    double b1_2;
    double b1_3;
    bool isStable = false;

    do {
        std::cout << "\n--- Model 1.7 (Linear) ---\n";
        std::cout << "Enter a, b1, b2, b3: ";
        std::cin >> a1 >> b1_1 >> b1_2 >> b1_3;

        if (std::abs(a1) >= 1.0) {
            std::cout << "[WARNING] System is UNSTABLE (|a| >= 1). Please re-enter coefficients.\n";
        }
        else {
            isStable = true;
        }
    } while (!isStable);

    Model1_7 m1(a1, b1_1, b1_2, b1_3);

    double a2;
    double b2;
    std::cout << "\n--- Model 2.9 (Non-linear) ---\n";
    std::cout << "Enter a, b: ";
    std::cin >> a2 >> b2;
    Model2_9 m2(a2, b2);

    double a3;
    double dt;
    std::cout << "\n--- Model 3.1 (Differential) ---\n";
    std::cout << "Enter a, dt: ";
    std::cin >> a3 >> dt;
    Model3_1 m3(a3, dt);

    std::array<model_standard*, 3> models = { &m1, &m2, &m3 };
    std::array<const char*, 3> names = { "Model1_7", "Model2_9", "Model3_1" };
    std::array<const char*, 3> inputs = { "step", "impulse", "harmonic" };

    std::cout << "\nStarting simulation...\n";

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::string filename = std::string(names[i]) + "_" + inputs[j] + ".csv";
            simulateAndSave(models[i], filename, n, j, names[i]);
        }
    }

    std::cout << "\nSimulation finished. CSV files generated in build folder.\n";
    return 0;
}