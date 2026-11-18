#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#include "DiffEquations.h"
#include "InputSignal.h"
#include "LinearModels.h"
#include "Model.h"
#include "NonLinearModels.h"
#include "Simulator.h"

namespace
{

// Параметры по умолчанию (подобраны так, чтобы все процессы были устойчивыми).
struct Defaults
{
    // Model 1.4
    static constexpr double a14 = 0.8;
    static constexpr double b14 = 1.0;
    // Model 2.6
    static constexpr double a1_26 = 0.1;
    static constexpr double a2_26 = 0.3;
    static constexpr double b26 = 0.5;
    // Model 3.8
    static constexpr double a1_38 = 0.5;
    static constexpr double a2_38 = 0.1;
    static constexpr double b38 = 1.0;
    static constexpr double dt38 = 0.1;

    static constexpr int n = 30;
    static constexpr double y0 = 0.0;
    static constexpr double amplitude = 1.0;
};

// Чтение строки; пустой ввод (Enter) возвращает значение по умолчанию.
double readDouble(const std::string& prompt, double def)
{
    while (true)
    {
        std::cout << prompt << " [" << def << "]: ";
        std::string line;
        if (!std::getline(std::cin, line))
            return def;
        if (line.empty())
            return def;

        std::istringstream in(line);
        if (double value = 0.0; in >> value && std::isfinite(value))
            return value;

        std::cout << "Ошибка ввода, введите число.\n";
    }
}

int readInt(const std::string& prompt, int def, int minValue, int maxValue)
{
    while (true)
    {
        std::cout << prompt << " [" << def << "]: ";
        std::string line;
        if (!std::getline(std::cin, line))
            return def;
        if (line.empty())
            return def;

        std::istringstream in(line);
        if (int value = 0; in >> value && value >= minValue && value <= maxValue)
            return value;

        std::cout << "Ошибка ввода, введите целое число от " << minValue
                  << " до " << maxValue << ".\n";
    }
}

std::unique_ptr<Model> createModel(int choice, bool askParams)
{
    switch (choice)
    {
    case 1:
    {
        double a = Defaults::a14;
        double b = Defaults::b14;
        if (askParams)
        {
            a = readDouble("a", a);
            b = readDouble("b", b);
        }
        return std::make_unique<Model1_4>(a, b);
    }
    case 2:
    {
        double a1 = Defaults::a1_26;
        double a2 = Defaults::a2_26;
        double b = Defaults::b26;
        if (askParams)
        {
            a1 = readDouble("a1", a1);
            a2 = readDouble("a2", a2);
            b = readDouble("b", b);
        }
        return std::make_unique<Model2_6>(a1, a2, b);
    }
    default:
    {
        double a1 = Defaults::a1_38;
        double a2 = Defaults::a2_38;
        double b = Defaults::b38;
        double dt = Defaults::dt38;
        if (askParams)
        {
            a1 = readDouble("a1", a1);
            a2 = readDouble("a2", a2);
            b = readDouble("b", b);
            dt = readDouble("dt (шаг интегрирования)", dt);
        }
        return std::make_unique<Model3_8>(a1, a2, b, dt);
    }
    }
}

std::unique_ptr<InputSignal> createSignal(int choice, double amplitude)
{
    switch (choice)
    {
    case 1: return std::make_unique<StepSignal>(amplitude);
    case 2: return std::make_unique<ImpulseSignal>(amplitude);
    default: return std::make_unique<HarmonicSignal>(amplitude);
    }
}

// Имя модели для файлов результатов. Номер проверяется через switch,
// поэтому выход за границы невозможен.
std::string modelId(int modelChoice)
{
    switch (modelChoice)
    {
    case 1: return "model_1_4";
    case 2: return "model_2_6";
    default: return "model_3_8";
    }
}

// Возвращает имя CSV-файла с результатом.
std::string simulateAndSave(int modelChoice, Model& model,
                            const InputSignal& signal, int n, double y0)
{
    std::cout << "\n============================================\n";
    std::cout << model.name() << "\n" << model.formula() << '\n';
    std::cout << "Входное воздействие: " << signal.name() << '\n';

    if (const auto* linear = dynamic_cast<const Model1_4*>(&model))
    {
        if (!linear->isStable())
            std::cout << "ВНИМАНИЕ: |a| >= 1, корень характеристического "
                         "уравнения z = a вне единичного круга - "
                         "процесс неустойчив!\n";
    }
    std::cout << "--------------------------------------------\n";

    const auto rows = Simulator::run(model, signal, n, y0);
    Simulator::printTable(rows);

    const std::string file = modelId(modelChoice) + "_" + signal.id() + ".csv";
    if (Simulator::saveCsv(rows, file))
        std::cout << "Сохранено в " << file << '\n';
    return file;
}

// Без аргумента plot.py строит графики всех моделей,
// с именем файла - один график только для этого файла.
void runPlot(const std::string& csvFile = "")
{
    if (!std::ifstream("plot.py"))
    {
        std::cout << "\nplot.py не найден рядом с программой, "
                     "графики не построены.\n";
        return;
    }

    std::cout << "\nПостроение графиков (plot.py)...\n" << std::flush;
    // Аргумент для plot.py: пусто или " model_1_4_step.csv"
    const std::string args = csvFile.empty() ? std::string() : " " + csvFile;
#ifdef _WIN32
    const std::string command =
        "python plot.py" + args + " || py plot.py" + args;
#else
    const std::string command = "python3 plot.py" + args;
#endif
    const int code = std::system(command.c_str());
    if (code != 0)
        std::cout << "Не удалось запустить Python. Установите Python и "
                     "matplotlib (pip install matplotlib) и запустите "
                     "plot.py вручную.\n";
}

} // namespace

int main()
{
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    std::cout << "============================================\n";
    std::cout << " OTIS-2026 | Лабораторная работа №1 | Вариант 4\n";
    std::cout << " Model 1.4, Model 2.6, Model 3.8\n";
    std::cout << "============================================\n";
    std::cout << "(Enter - принять значение по умолчанию в скобках)\n\n";

    if (const int mode = readInt(
            "Режим:\n"
            "  1 - все модели x все воздействия (параметры по умолчанию)\n"
            "  2 - одна модель, свои параметры\n"
            "Ваш выбор", 1, 1, 2);
        mode == 1)
    {
        const int n = readInt("Количество шагов n", Defaults::n, 1, 100000);

        for (int m = 1; m <= 3; ++m)
        {
            auto model = createModel(m, false);
            for (int s = 1; s <= 3; ++s)
            {
                auto signal = createSignal(s, Defaults::amplitude);
                simulateAndSave(m, *model, *signal, n, Defaults::y0);
            }
        }
        runPlot();
    }
    else
    {
        const int m = readInt(
            "Модель:\n"
            "  1 - Model 1.4 (линейная)\n"
            "  2 - Model 2.6 (нелинейная)\n"
            "  3 - Model 3.8 (дифф. уравнение)\n"
            "Ваш выбор", 1, 1, 3);
        auto model = createModel(m, true);

        const int s = readInt(
            "Входное воздействие:\n"
            "  1 - ступенчатое\n"
            "  2 - импульсное\n"
            "  3 - гармоническое\n"
            "Ваш выбор", 1, 1, 3);
        // По заданию импульс единичный (u0 = 1), амплитуду для него не спрашиваем.
        const double amplitude =
            (s == 2) ? 1.0 : readDouble("Амплитуда A", Defaults::amplitude);
        auto signal = createSignal(s, amplitude);

        const int n = readInt("Количество шагов n", Defaults::n, 1, 100000);
        const double y0 = readDouble("Начальное значение y0", Defaults::y0);

        const std::string file = simulateAndSave(m, *model, *signal, n, y0);
        runPlot(file);
    }

    return 0;
}
