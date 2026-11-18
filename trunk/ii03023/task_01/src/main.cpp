#include <iostream>
#include <fstream>
#include <iomanip>
#include <memory>
#include <vector>
#include "Model1_6.h"
#include "Model2_10.h"
#include "Model3_4.h"

// Сохранение результатов и входного сигнала в CSV-файл для визуализации
void saveToCsv(const std::string& filename, const std::vector<double>& input_data, const std::vector<double>& model_data) {
    std::string fullPath = "../../" + filename; 
    
    std::ofstream file(fullPath);
    if (!file.is_open()) {
        fullPath = "../" + filename;
        file.open(fullPath);
    }
    
    if (!file.is_open()) {
        fullPath = filename;
        file.open(fullPath);
    }

    file << "Step,Input,Value\n";
    for (size_t i = 0; i < model_data.size(); ++i) {
        file << i << "," << input_data[i] << "," << model_data[i] << "\n";
    }
    file.close();
    std::cout << "\n[INFO] Результаты успешно сохранены в: " << fullPath << "\n";
}

int main() {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    int steps = 50;
    std::cout << "Введите количество шагов симуляции (по умолчанию 50): ";
    if (std::cin >> steps && steps <= 0) {
        steps = 50;
    }

    // 1. ВЫБОР МОДЕЛИ
    std::cout << "\n========================================" << std::endl;
    std::cout << "        ВЫБОР МАТЕМАТИЧЕСКОЙ МОДЕЛИ     " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. Модель 1.6 (Линейная дискретная модель)" << std::endl;
    std::cout << "2. Модель 2.10 (Нелинейная с гиперболическим тангенсом)" << std::endl;
    std::cout << "3. Модель 3.4 (Квадратичный самораспад)" << std::endl;
    std::cout << "Введите номер модели (1-3): ";
    
    int modelChoice = 1;
    std::cin >> modelChoice;

    std::unique_ptr<Model> model;
    std::string modelName = "";

    // 2. ВВОД ПАРАМЕТРОВ УРАВНЕНИЯ
    if (modelChoice == 1) {
        double a1;
        double a2;
        double a3;
        double b;
        std::cout << "\n--- Ввод коэффициентов для Модели 1.6 ---\n";
        std::cout << "Введите a1: "; std::cin >> a1;
        std::cout << "Введите a2: "; std::cin >> a2;
        std::cout << "Введите a3: "; std::cin >> a3;
        std::cout << "Введите b: ";  std::cin >> b;
        model = std::make_unique<Model1_6>(a1, a2, a3, b);
        modelName = "Модель 1.6";
    } 
    else if (modelChoice == 2) {
        double a;
        double b;
        std::cout << "\n--- Ввод коэффициентов для Модели 2.10 ---\n";
        std::cout << "Введите a: "; std::cin >> a;
        std::cout << "Введите b: "; std::cin >> b;
        model = std::make_unique<Model2_10>(a, b);
        modelName = "Модель 2.10";
    } 
    else {
        double a;
        double dt;
        std::cout << "\n--- Ввод коэффициентов для Модели 3.4 ---\n";
        std::cout << "Введите a (параметр затухания): "; std::cin >> a;
        std::cout << "Введите dt (шаг дискретизации): "; std::cin >> dt;
        model = std::make_unique<Model3_4>(a, dt);
        modelName = "Модель 3.4";
    }

    // 3. ВЫБОР ТИПА ВХОДНОГО СИГНАЛА
    std::cout << "\n========================================" << std::endl;
    std::cout << "        ВЫБОР ВХОДНОГО СИГНАЛА          " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. Ступенчатый (STEP)" << std::endl;
    std::cout << "2. Импульсный (IMPULSE)" << std::endl;
    std::cout << "3. Гармонический (HARMONIC)" << std::endl;
    std::cout << "Введите тип сигнала (1-3): ";

    int signalChoice = 1;
    std::cin >> signalChoice;

    InputType inputType = InputType::STEP;
    std::string signalName = "Ступенчатый";
    if (signalChoice == 2) {
        inputType = InputType::IMPULSE;
        signalName = "Импульсный";
    } else if (signalChoice == 3) {
        inputType = InputType::HARMONIC;
        signalName = "Гармонический";
    }

    // Запуск симуляции
    std::vector<double> results = model->simulate(steps, inputType);

    // Сбор значений входного сигнала для каждого шага
    std::vector<double> inputValues;
    for (int i = 0; i < steps; ++i) {
        inputValues.push_back(Model::getSignal(inputType, i));
    }

    std::cout << "\n[INFO] Симуляция завершена успешно!" << std::endl;
    std::cout << "       Модель: " << modelName << std::endl;
    std::cout << "       Сигнал: " << signalName << std::endl;

    // Сохранение в CSV
    saveToCsv("simulation_results.csv", inputValues, results);

    std::cout << "\n[SUCCESS] Готово! Запустите 'build.bat' для отображения графика!" << std::endl;
    return 0;
}