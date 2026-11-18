#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "model.h"
#include "model19.h"
#include "model23.h"
#include "model37.h"

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
        double u = 0.0;

        if (signalChoice == 1) {
            u = amplitude;
        }
        else if (signalChoice == 2 && tau == 0) {
            u = amplitude;
        }
        else if (signalChoice == 3) {
            u = amplitude * std::sin(tau);
        }

        double y = model.nextStep(u);

        std::cout << tau << "\t| " << u << "\t| " << y << "\n";
        csvFile << model.getName() << ";" << signalType << ";" << tau << ";" << u << ";" << y << "\n";
    }

    std::cout << "Результаты записаны в simulation_results.csv\n";
}

int main() {
    std::ofstream csvFile("simulation_results.csv");

    if (!csvFile.is_open()) {
        std::cout << "Ошибка открытия файла.\n";
        return 1;
    }

    csvFile << "Model;SignalType;Step;U;Y\n";

    while (true) {
        int modelChoice = 0;

        std::cout << "\nМодель: 1 - 1.9, 2 - 2.3, 3 - 3.7, 0 - выход\n";
        std::cout << "Ваш выбор: ";
        std::cin >> modelChoice;

        if (!std::cin || modelChoice == 0) {
            break;
        }

        if (modelChoice == 1) {
            double a1 = 0.0;
            double a2 = 0.0;
            double a3 = 0.0;
            double b1 = 0.0;

            std::cout << "a1 = ";
            std::cin >> a1;
            std::cout << "a2 = ";
            std::cin >> a2;
            std::cout << "a3 = ";
            std::cin >> a3;
            std::cout << "b1 = ";
            std::cin >> b1;

            Model19 model(a1, a2, a3, b1);
            runSimulation(model, csvFile);
        }
        else if (modelChoice == 2) {
            double a = 0.0;
            double b = 0.0;
            double delta = 0.0;

            std::cout << "a = ";
            std::cin >> a;
            std::cout << "b = ";
            std::cin >> b;
            std::cout << "delta = ";
            std::cin >> delta;

            Model23 model(a, b, delta);
            runSimulation(model, csvFile);
        }
        else if (modelChoice == 3) {
            double a = 0.0;
            double b = 0.0;
            double dt = 0.0;

            std::cout << "a = ";
            std::cin >> a;
            std::cout << "b = ";
            std::cin >> b;
            std::cout << "dt = ";
            std::cin >> dt;

            Model37 model(a, b, dt);
            runSimulation(model, csvFile);
        }
        else {
            std::cout << "Неверный выбор модели.\n";
        }
    }

    return 0;
}
