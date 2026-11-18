#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <cmath>
#include <iomanip>
#include <limits>

#include "Model.h"
#include "Model 1.3.h"
#include "Model 2.10.h"
#include "Model 3.2.h"

int getValidInt(const std::string& prompt, int minValue) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minValue) {
            return value;
        } else {
            std::cout << "Ошибка! Введите целое число >= " << minValue << ". Попробуйте снова.\n";
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        }
    }
}

double getValidDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        } else {
            std::cout << "Ошибка! Введите число (можно дробное). Попробуйте снова.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

double generateSignal(int type, int t) {
    switch(type) {
        case 0: return 1.0;
        case 1: return (t == 0) ? 1.0 : 0.0;
        case 2: return std::sin(t);
        default: return 0.0;
    }
}

std::string getSignalName(int type) {
    switch(type) {
        case 0: return "Step";
        case 1: return "Impulse";
        case 2: return "Harmonic";
        default: return "Unknown";
    }
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
    std::cout << "=== ЛАБОРАТОРНАЯ РАБОТА №1 (Вариант 27) ===" << std::endl;

    int modelChoice;
    std::cout << "\nВыберите модель для симуляции:" << std::endl;
    std::cout << "1. Линейная модель (Model 1.3)" << std::endl;
    std::cout << "2. Нелинейная модель (Model 2.10)" << std::endl;
    std::cout << "3. Дифференциальное уравнение (Model 3.2)" << std::endl;
    
    while (true) {
        std::cout << "Ваш выбор (1-3): ";
        if (std::cin >> modelChoice && modelChoice >= 1 && modelChoice <= 3) {
            break;
        } else {
            std::cout << "Ошибка! Введите число от 1 до 3." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    int steps = getValidInt("Введите количество шагов симуляции n (> 0): ", 1);

    std::ofstream csvFile("simulation_results.csv");
    csvFile << "Model,Signal,Step,u,y\n";

    std::shared_ptr<Model> selectedModel;
    std::string modelName;

    if (modelChoice == 1) {
        std::cout << "\n--- Линейная модель 1.3 ---\n";
        std::cout << "y(t+1) = a1*y(t) + a2*y(t-1) + b*u(t)\n";
        double a1 = getValidDouble("  Введите a1: ");
        double a2 = getValidDouble("  Введите a2: ");
        double b = getValidDouble("  Введите b:  ");
        selectedModel = std::make_shared<LinearModel13>(a1, a2, b);
        modelName = "Model_1.3";
    } 
    else if (modelChoice == 2) {
        std::cout << "\n--- Нелинейная модель 2.10 ---\n";
        std::cout << "y(t+1) = a*tanh(y(t)) + b*u(t)^3\n";
        double a = getValidDouble("  Введите a: ");
        double b = getValidDouble("  Введите b: ");
        selectedModel = std::make_shared<NonlinearModel2_10>(a, b);
        modelName = "Model_2.10";
    } 
    else if (modelChoice == 3) {
        std::cout << "\n--- Дифференциальное уравнение 3.2 ---\n";
        std::cout << "dy/dt = b*u\n";
        double b = getValidDouble("  Введите b:  ");
        
        double dt;
        while (true) {
            dt = getValidDouble("  Введите шаг времени dt (> 0): ");
            if (dt > 0.0) break;
            std::cout << "Ошибка! dt должен быть больше нуля.\n";
        }
        selectedModel = std::make_shared<DiffEqModel3_2>(b, dt);
        modelName = "Model_3.2";
    }

    std::cout << "\nЗапуск симуляции для " << modelName << "..." << std::endl;
    for (int sigType = 0; sigType < 3; ++sigType) {
        runSimulation(selectedModel, steps, sigType, modelName, csvFile);
    }

    csvFile.close();
    std::cout << "\nСимуляция завершена. Данные сохранены в simulation_results.csv" << std::endl;
    return 0;
}