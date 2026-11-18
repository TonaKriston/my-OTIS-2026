#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

#include "InputSignal.h"
#include "Model.h"
#include "Model1_6.h"
#include "Model2_8.h"
#include "Model3_10.h"

struct Line
{
    int tau;
    double u;
    double y;
};

static void writeCsv(const std::vector<Line>& lines)
{
    std::ofstream file("results.csv");
    if (!file)
    {
        std::cerr << "Не удалось открыть results.csv для записи.\n";
        return;
    }
    file << "tau,u,y\n";
    for (const Line& line : lines)
        file << line.tau << ',' << line.u << ',' << line.y << '\n';
    std::cout << "\nРезультаты сохранены в results.csv\n";
}


static int readInteger(const std::string& prompt, int minValue)
{
    int value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value && value >= minValue)
            return value;
        std::cout << "Ошибка ввода. Повторите.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

static double readReal(const std::string& prompt)
{
    double value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value && std::isfinite(value))
            return value;
        std::cout << "Ошибка ввода. Повторите.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

static SignalType choiseSignalType()
{
    while (true)
    {
        if (const int choice = readInteger(
            "\nТип входного воздействия:\n"
            "1 - ступенчатое\n"
            "2 - импульсное\n"
            "3 - гармоническое\n"
            "Ваш выбор: ", 1); choice <= 3)
            return static_cast<SignalType>(choice);

        std::cout << "Нет такого варианта. Повторите.\n";
    }
}

static std::unique_ptr<Model> createModel()
{
    while (true)
    {
        const int choice = readInteger(
            "\nВыберите модель:\n"
            "1 - Model 1.6\n"
            "2 - Model 2.8\n"
            "3 - Model 3.10\n"
            "Ваш выбор: ", 1);

        switch (choice)
        {
        case 1:
        {
            const double a1 = readReal("a1 = ");
            const double a2 = readReal("a2 = ");
            const double a3 = readReal("a3 = ");
            const double b  = readReal("b  = ");
            return std::make_unique<Model1_6>(a1, a2, a3, b);
        }
        case 2:
        {
            const double a = readReal("a = ");
            const double b = readReal("b = ");
            const double c = readReal("c = ");
            return std::make_unique<Model2_8>(a, b, c);
        }
        case 3:
        {
            const double a = readReal("a = ");
            const double b = readReal("b = ");
            return std::make_unique<Model3_10>(a, b);
        }
        default:
            std::cout << "Нет такой модели. Повторите выбор.\n";
        }
    }
}

static void printTable(const std::vector<Line>& lines)
{
    std::cout << "\n" << std::setw(8) << "tau"
              << std::setw(15) << "u"
              << std::setw(15) << "y" << '\n';
    std::cout << std::string(38, '-') << '\n';
    for (const Line& line : lines)
    {
        std::cout << std::setw(8) << line.tau
                  << std::setw(15) << std::fixed << std::setprecision(6) << line.u
                  << std::setw(15) << line.y << '\n';
    }
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
    std::cout << "============================================\n";
    std::cout << " OTIS-2026 | Лабораторная работа №1\n";
    std::cout << " Вариант 6, Низамутдинов М.А.\n";
    std::cout << "============================================\n";

    std::unique_ptr<Model> model = createModel();
    const SignalType signal = choiseSignalType();
    const double amplitude = readReal("Амплитуда A = ");
    const int n = readInteger("Количество шагов n = ", 1);
    const double y0 = readReal("Начальное значение y0 = ");

    double dt = 1.0;
    if (std::string(model->getName()).find("Model 3.10") != std::string::npos)
        dt = readReal("Шаг интегрирования dt = ");

    std::vector<Line> lines;
    lines.reserve(n + 1);
    double y = y0;
    double yPrev = y0;
    double yPrev2 = y0;

    lines.push_back({0, generalInput(signal, 0, amplitude), y});

    for (int tau = 0; tau < n; ++tau)
    {
        const double u = generalInput(signal, tau, amplitude);
        const double nextY = model->calculateNext(y, yPrev, yPrev2, u, dt);
        yPrev2 = yPrev;
        yPrev = y;
        y = nextY;
        lines.push_back({tau + 1, generalInput(signal, tau + 1, amplitude), y});
    }

    std::cout << "\nМодель: " << model->getName() << '\n';
    printTable(lines);
    writeCsv(lines);

    system("python plot.py");

    std::cout << "\nНажмите Enter для выхода...";
    std::cin.ignore();
    std::cin.get();

    return 0;
}
