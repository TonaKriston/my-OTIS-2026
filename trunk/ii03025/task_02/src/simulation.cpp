#include "simulation.h"
#include <cmath>
#include <iostream>
#include <string>

double generateSignal(int signalChoice, double amplitude, int tau) {
    if (signalChoice == 1) return amplitude;
    if (signalChoice == 2 && tau == 0) return amplitude;
    if (signalChoice == 3) return amplitude * std::sin(tau);
    return 0.0;
}

void runSimulation(Model& model, std::ofstream& csvFile) {
    int signalChoice = 0;
    double amplitude = 0.0;
    int n = 0;

    std::cout << "Сигнал (1 - ступенчатый, 2 - импульсный, 3 - гармонический): ";
    std::cin >> signalChoice;

    std::cout << "Амплитуда A = ";
    std::cin >> amplitude;

    std::cout << "Количество шагов n = ";
    std::cin >> n;

    std::string signalType;

    if (signalChoice == 1) {
        signalType = "Step";
    }
    else if (signalChoice == 2) {
        signalType = "Pulse";
    }
    else if (signalChoice == 3) {
        signalType = "Harmonic";
    }
    else {
        std::cout << "Неверный выбор сигнала.\n";
        return;
    }

    std::cout << "tau\t| U\t| Y\n";

    for (int tau = 0; tau < n; ++tau) {
        double u = generateSignal(signalChoice, amplitude, tau);
        double y = model.nextStep(u);

        std::cout << tau << "\t| " << u << "\t| " << y << "\n";

        csvFile << model.getName() << ";" << signalType << ";"
                << tau << ";" << u << ";" << y << "\n";
    }

    std::cout << "Результаты записаны в simulation_results.csv\n";
}
