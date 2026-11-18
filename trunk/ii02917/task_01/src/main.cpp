#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

#include "Model.h"
#include "Model1_4.h"
#include "Model2_8.h"
#include "Model3_2.h"
#include "InputSignal.h"

struct SimulationRecord {
    int index;
    double inputVal;
    double outputVal;
};

std::vector<SimulationRecord> runSimulation(ISystemModel& targetModel, const SignalGenerator& sourceSignal, int totalIterations) {
    std::vector<SimulationRecord> trajectory;
    targetModel.clearState();

    for (int step = 0; step < totalIterations; step++) {
        double currentU = sourceSignal.computeValue(step);
        double currentY = targetModel.calculateNext(currentU, step);
        trajectory.push_back({step, currentU, currentY});
    }
    return trajectory;
}

void showOutputTable(const std::string& systemTitle, const std::vector<SimulationRecord>& dataLog, int itemsToShow = 20) {
    std::cout << "\n>>> " << systemTitle << " <<<" << std::endl;
    std::cout << std::setw(8) << "Шаг"
              << std::setw(15) << "Вход u"
              << std::setw(15) << "Выход y" << std::endl;
    std::cout << std::string(38, '=') << std::endl;

    int limit = std::min(static_cast<int>(dataLog.size()), itemsToShow);
    for (int i = 0; i < limit; i++) {
        std::cout << std::setw(8) << dataLog[i].index
                  << std::setw(15) << std::fixed << std::setprecision(6) << dataLog[i].inputVal
                  << std::setw(15) << dataLog[i].outputVal << std::endl;
    }
    if (static_cast<int>(dataLog.size()) > itemsToShow) {
        std::cout << "  ... [Отображены первые " << itemsToShow << " из " << dataLog.size() << " строк лога]" << std::endl;
    }
}

void exportLogToCSV(const std::string& path, const std::string& systemTitle, const std::vector<SimulationRecord>& dataLog) {
    std::ofstream stream(path, std::ios::app);
    if (!stream.is_open()) {
        std::cerr << "Не удалось открыть целевой файл: " << path << std::endl;
        return;
    }

    stream << "\n# " << systemTitle << "\n";
    stream << "step,u,y\n";
    for (const auto& record : dataLog) {
        stream << record.index << "," << std::fixed << std::setprecision(6) << record.inputVal << "," << record.outputVal << "\n";
    }
    stream.close();
}

int main() {
    std::cout << "********************************************" << std::endl;
    std::cout << "  Лабораторная работа 01 (Вариант 14)" << std::endl;
    std::cout << "  Исследование систем: 1.4, 2.8, 3.2" << std::endl;
    std::cout << "********************************************" << std::endl;

    int stepsCount = 50;
    std::cout << "\nЗадайте число шагов интегрирования (по умолчанию 50): ";
    std::string line;
    std::getline(std::cin, line);
    if (!line.empty()) {
        try { 
            stepsCount = std::stoi(line); 
            if (stepsCount < 1) stepsCount = 1;
            if (stepsCount > 10000) stepsCount = 10000;
        } catch (...) { 
            stepsCount = 50; 
        }
    }
    std::cout << "Установлено шагов: " << stepsCount << std::endl;

    std::cout << "\nКонфигурация входного воздействия:" << std::endl;
    std::cout << "  1 - Постоянный уровень (Ступенька)" << std::endl;
    std::cout << "  2 - Мгновенный импульс" << std::endl;
    std::cout << "  3 - Синусоидальное воздействие" << std::endl;
    std::cout << "Ваш выбор: ";
    int modeChoice = 1;
    std::getline(std::cin, line);
    if (!line.empty()) {
        try { modeChoice = std::stoi(line); } catch (...) { modeChoice = 1; }
    }

    SourceType selectedMode;
    switch (modeChoice) {
        case 2: selectedMode = SourceType::DELTA; break;
        case 3: selectedMode = SourceType::SINUSOIDAL; break;
        default: selectedMode = SourceType::CONSTANT; break;
    }

    SignalGenerator generator(selectedMode, 1.0, 1.0);
    std::cout << "Выбранный сигнал: " << generator.retrieveFormatName() << std::endl;

    std::ofstream initializeStorage("results.csv", std::ios::trunc);
    initializeStorage << "Результаты моделирования. Вариант 14\n";
    initializeStorage << "Тип воздействия: " << generator.retrieveFormatName() << "\n";
    initializeStorage << "Количество итераций: " << stepsCount << "\n\n";
    initializeStorage.close();

    std::cout << "\n\n<<< ТЕСТИРОВАНИЕ ОБЪЕКТА 1.4 >>>" << std::endl;
    {
        Model1_4 activeModel(0.8, 0.5);
        activeModel.displayStabilityReport();

        if (!activeModel.checkStability()) {
            std::cout << "\n  [Внимание] Объект 1.4 классифицирован как НЕУСТОЙЧИВЫЙ! Траектория будет расходящейся." << std::endl;
        }

        auto simulationData = runSimulation(activeModel, generator, stepsCount);
        showOutputTable(activeModel.fetchName(), simulationData);
        exportLogToCSV("results.csv", activeModel.fetchName(), simulationData);
    }

    std::cout << "\n\n<<< ТЕСТИРОВАНИЕ ОБЪЕКТА 2.8 >>>" << std::endl;
    {
        Model2_8 activeModel(3.8, 0.1, 0.05);
        std::cout << "  Нелинейная структура — проверка устойчивости выполняется по результатам симуляции." << std::endl;

        auto simulationData = runSimulation(activeModel, generator, stepsCount);
        activeModel.displayStabilityReport();

        if (!activeModel.checkStability()) {
            std::cout << "\n  [Внимание] Объект 2.8 классифицирован как НЕУСТОЙЧИВЫЙ!" << std::endl;
        }

        showOutputTable(activeModel.fetchName(), simulationData);
        exportLogToCSV("results.csv", activeModel.fetchName(), simulationData);
    }

    std::cout << "\n\n<<< ТЕСТИРОВАНИЕ ОБЪЕКТА 3.2 >>>" << std::endl;
    {
        Model3_2 activeModel(1.0, 0.1);
        activeModel.displayStabilityReport();

        auto simulationData = runSimulation(activeModel, generator, stepsCount);
        showOutputTable(activeModel.fetchName(), simulationData);
        exportLogToCSV("results.csv", activeModel.fetchName(), simulationData);
    }
    std::cout << "\nРасчеты завершены! Результаты сохранены в results.csv" << std::endl;
    std::cout << "Запуск построения графиков..." << std::endl;

    system("pause");

    return 0;
}