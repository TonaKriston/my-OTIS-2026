#include <cmath>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#include "ApplicationLogic.h"
#include "InputSignal.h"
#include "Model.h"
#include "Model1_3.h"
#include "Model2_5.h"
#include "Model3_7.h"
#include "Simulation.h"

// Модели и входные сигналы берутся из task_01, здесь только консольный ввод/вывод.

// Отдельный тип исключения: поток ввода закрыт, запрашивать значение больше не у кого.
class InputClosedError : public std::runtime_error
{
public:
    InputClosedError() : std::runtime_error("Ввод завершён (EOF).") {}
};

static int readInt(const std::string& prompt, int minValue)
{
    int value;

    while (true)
    {
        std::cout << prompt;

        if (std::cin >> value && value >= minValue)
            return value;

        if (std::cin.eof())
            throw InputClosedError();

        std::cout << "Ошибка ввода. Повторите.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

static double readDouble(const std::string& prompt)
{
    double value;

    while (true)
    {
        std::cout << prompt;

        if (std::cin >> value && std::isfinite(value))
            return value;

        if (std::cin.eof())
            throw InputClosedError();

        std::cout << "Ошибка ввода. Повторите.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

static SignalType readSignalType()
{
    while (true)
    {
        if (const int choice = readInt(
                "\nТип входного воздействия:\n"
                "1 - ступенчатое\n"
                "2 - импульсное\n"
                "3 - гармоническое\n"
                "Ваш выбор: ", 1);
            isValidSignalChoice(choice))
        {
            return static_cast<SignalType>(choice);
        }

        std::cout << "Нет такого воздействия. Повторите выбор.\n";
    }
}

static std::unique_ptr<Model> createModel()
{
    while (true)
    {
        const int choice = readInt(
            "\nВыберите модель:\n"
            "1 - Model 1.3\n"
            "2 - Model 2.5\n"
            "3 - Model 3.7\n"
            "Ваш выбор: ", 1);

        if (!isValidModelChoice(choice))
        {
            std::cout << "Нет такой модели. Повторите выбор.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            const double a1 = readDouble("a1 = ");
            const double a2 = readDouble("a2 = ");
            const double b = readDouble("b = ");

            // Анализ устойчивости: предупреждение выводится ДО начала симуляции.
            printStabilityReport(std::cout, a1, a2);

            return std::make_unique<Model1_3>(a1, a2, b);
        }

        case 2:
        {
            const double a = readDouble("a = ");
            const double b = readDouble("b = ");
            return std::make_unique<Model2_5>(a, b);
        }

        default:
        {
            const double a = readDouble("a = ");
            const double b = readDouble("b = ");
            return std::make_unique<Model3_7>(a, b);
        }
        }
    }
}

#ifndef OTIS_TESTING
int main()
{
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    try
    {
        std::cout << "============================================\n";
        std::cout << " OTIS-2026 | Лабораторная работа №2\n";
        std::cout << " Вариант 3 (модели и сигналы из task_01)\n";
        std::cout << "============================================\n";

        const std::unique_ptr<Model> model = createModel();

        const SignalType signal = readSignalType();
        const double amplitude = readDouble("Амплитуда A = ");
        const int n = readInt("Количество шагов n = ", 1);
        const double y0 = readDouble("Начальное значение y0 = ");

        // dt используется только Model 3.7.
        double dt = 1.0;
        if (requiresIntegrationStep(model->getName()))
            dt = readDouble("Шаг интегрирования dt = ");

        const std::vector<Row> rows = simulate(*model, signal, amplitude, n, y0, dt);

        std::cout << "\nМодель: " << model->getName() << '\n';
        printResultsTable(std::cout, rows);

        if (saveResultsCsv("results.csv", rows))
        {
            std::cout << "\nРезультаты сохранены в results.csv\n";
#ifdef _WIN32
            std::system("python plot.py");
#else
            std::system("python3 plot.py");
#endif
        }
        else
        {
            std::cerr << "Не удалось открыть results.csv для записи.\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Ошибка: " << e.what() << '\n';
        return 1;
    }

#ifdef _WIN32
    std::system("pause");
#endif
    return 0;
}
#endif // OTIS_TESTING
