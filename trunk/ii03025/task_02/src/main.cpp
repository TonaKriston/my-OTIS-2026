#include <fstream>
#include <iostream>
#include "stability.h"
#include "simulation.h"
#include "../../task_01/src/model.h"
#include "../../task_01/src/model19.h"
#include "../../task_01/src/model23.h"
#include "../../task_01/src/model37.h"

int main() {
    std::ofstream csvFile("simulation_results.csv");

    if (!csvFile.is_open()) {
        std::cout << "Ошибка открытия файла.\n";
        return 1;
    }

    csvFile << "Model;SignalType;Step;U;Y\n";

    int modelChoice = 0;

    do {
        std::cout << "\nМодель: 1 - 1.9, 2 - 2.3, 3 - 3.7, 0 - выход\n";
        std::cout << "Ваш выбор: ";
        std::cin >> modelChoice;

        if (!std::cin || modelChoice == 0) {
            break;
        }

        if (modelChoice == 1) {
            double a1;
            double a2;
            double a3;
            double b1;

            std::cout << "a1 = ";
            std::cin >> a1;
            std::cout << "a2 = ";
            std::cin >> a2;
            std::cout << "a3 = ";
            std::cin >> a3;
            std::cout << "b1 = ";
            std::cin >> b1;

            Model19 model(a1, a2, a3, b1);

            if (!isStable19(a1, a2, a3)) {
                std::cout << "ВНИМАНИЕ: система неустойчива!\n";
            }

            runSimulation(model, csvFile);
        }
        else if (modelChoice == 2) {
            double a;
            double b;
            double delta;

            std::cout << "a = ";
            std::cin >> a;
            std::cout << "b = ";
            std::cin >> b;
            std::cout << "delta = ";
            std::cin >> delta;

            Model23 model(a, b, delta);

            if (!isStable23(a)) {
                std::cout << "ВНИМАНИЕ: система неустойчива!\n";
            }

            runSimulation(model, csvFile);
        }
        else if (modelChoice == 3) {
            double a;
            double b;
            double dt;

            std::cout << "a = ";
            std::cin >> a;
            std::cout << "b = ";
            std::cin >> b;
            std::cout << "dt = ";
            std::cin >> dt;

            Model37 model(a, b, dt);

            if (!isStable37(a, dt)) {
                std::cout << "ВНИМАНИЕ: система неустойчива!\n";
            }

            runSimulation(model, csvFile);
        }
        else {
            std::cout << "Неверный выбор модели.\n";
        }

    } while (true);

    return 0;
}
