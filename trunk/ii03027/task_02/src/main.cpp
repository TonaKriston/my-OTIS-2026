#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>
#include <limits>
#include <string>

#include "Model.h"
#include "LinearModel13.h"
#include "NonlinearModel2_10.h"
#include "DiffEqModel3_2.h"
#include "StabilityAnalyzer.h"

int getValidInt(const std::string& prompt, int minValue) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minValue) return value;
        std::cout << "Ошибка! Введите целое число >= " << minValue << ".\n";
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    }
}

double getValidDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) return value;
        std::cout << "Ошибка! Введите число.\n";
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

double generateSignal(int type, int t) {
    if (type == 0) return 1.0;
    if (type == 1) return (t == 0) ? 1.0 : 0.0;
    if (type == 2) return std::sin(t);
    return 0.0;
}

std::string getSignalName(int type) {
    if (type == 0) return "Step";
    if (type == 1) return "Impulse";
    if (type == 2) return "Harmonic";
    return "Unknown";
}

void runSimulation(const std::shared_ptr<Model>& model, int steps, int signalType, const std::string& modelName, std::ofstream& csvFile) {
    model->reset();
    std::string sigName = getSignalName(signalType);
    for (int t = 0; t < steps; ++t) {
        double u = generateSignal(signalType, t);
        double y = model->step(u);
        csvFile << modelName << "," << sigName << "," << t << "," << u << "," << y << "\n";
    }
}

int main() {
    std::cout << "=== ЛАБОРАТОРНАЯ РАБОТА №2 (Вариант 27) ===\n";
    int modelChoice;
    std::cout << "1. Линейная (1.3)\n2. Нелинейная (2.10)\n3. Диф. уравнение (3.2)\nВаш выбор (1-3): ";
    while (!(std::cin >> modelChoice) || modelChoice < 1 || modelChoice > 3) {
        std::cout << "Ошибка! Введите 1-3: "; std::cin.clear(); std::cin.ignore(10000, '\n');
    }

    int steps = getValidInt("Введите количество шагов n (> 0): ", 1);
    std::ofstream csvFile("simulation_results.csv");
    csvFile << "Model,Signal,Step,u,y\n";

    std::shared_ptr<Model> selectedModel;
    std::string modelName;

    if (modelChoice == 1) {
        double a1 = getValidDouble("Введите a1: ");
        double a2 = getValidDouble("Введите a2: ");
        double b = getValidDouble("Введите b: ");
        std::cout << "\n[Анализ устойчивости] Корни: " << StabilityAnalyzer::getRootsInfo(a1, a2) << "\n";
        if (!StabilityAnalyzer::isStable(a1, a2)) {
            std::cout << "\033[31mВНИМАНИЕ! Система НЕУСТОЙЧИВА!\033[0m\n";
        } else std::cout << "Система устойчива.\n";
        selectedModel = std::make_shared<LinearModel13>(a1, a2, b);
        modelName = "Model_1.3";
    } else if (modelChoice == 2) {
        double a = getValidDouble("Введите a: "), b = getValidDouble("Введите b: ");
        selectedModel = std::make_shared<NonlinearModel2_10>(a, b);
        modelName = "Model_2.10";
    } else {
        double b = getValidDouble("Введите b: ");
        double dt;
        while (true) { dt = getValidDouble("Введите dt (> 0): "); if (dt > 0) break; }
        selectedModel = std::make_shared<DiffEqModel3_2>(b, dt);
        modelName = "Model_3.2";
    }

    for (int sigType = 0; sigType < 3; ++sigType) runSimulation(selectedModel, steps, sigType, modelName, csvFile);
    csvFile.close();
    std::cout << "\nСимуляция завершена. Данные в simulation_results.csv\n";
    return 0;
}