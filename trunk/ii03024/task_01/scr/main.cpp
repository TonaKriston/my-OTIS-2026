#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdlib> // Для функции system() для запуска Gnuplot
#include <windows.h>
#include "model11.h"
#include "model28.h"
#include "model36.h"

// Функция для автоматического построения графика через Gnuplot
void showGraph(const std::string& filename, const std::string& titleName) {
    std::ofstream script("plot_temp.gnu");
    //script << "set encoding utf8\n";
    script << "set grid\n";
    script << "set title \"" << titleName << "\"\n";
    script << "set xlabel \"Шаг времени (tau)\"\n";
    script << "set ylabel \"Выход y(tau)\"\n";

    script << "set datafile separator \",\"\n";

    script << "plot '" << filename << "' skip 1 using 1:3 with lines linewidth 2 title 'y(tau)'\n";
    script << "pause -1 \"Нажмите Enter, чтобы закрыть график...\"\n";
    script.close();

    std::string command = "\"D:\\gnuplot\\bin\\gnuplot.exe\" plot_temp.gnu";
    std::system(command.c_str());
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::cout << "==================================================\n";
    std::cout << " ОТИС-2026 | Лабораторная работа №1\n";
    std::cout << " Вариант 26\n";
    std::cout << "==================================================\n\n";

    std::cout << "Выберите модель:\n";
    std::cout << "1 - Model 11 (Линейная)\n";
    std::cout << "2 - Model 28 (Нелинейная)\n";
    std::cout << "3 - Model 36 (Дифференциальная)\n";
    std::cout << "Ваш выбор: ";
    int modelChoice;
    std::cin >> modelChoice;

    Model* model = nullptr;
    std::string modelPrefix = "";
    std::string modelTitle = "";

    if (modelChoice == 1) {
        double a, b; // ИЗМЕНЕНО: теперь 2 коэффициента (a и b) вместо a1, a2, b
        std::cout << "a = "; std::cin >> a;
        std::cout << "b = "; std::cin >> b;
        model = new Model11(a, b);
        modelPrefix = "Model11";
        modelTitle = "Модель 11 (Линейная)";
    }
    else if (modelChoice == 2) {
        double a, b, c; // ИЗМЕНЕНО: теперь 3 коэффициента (a, b, c) для модели 2.8
        std::cout << "a = "; std::cin >> a;
        std::cout << "b = "; std::cin >> b;
        std::cout << "c = "; std::cin >> c;
        model = new Model28(a, b, c);
        modelPrefix = "Model28";
        modelTitle = "Модель 28 (Нелинейная)";
    }
    else if (modelChoice == 3) {
        double a, b;
        std::cout << "a = "; std::cin >> a;
        std::cout << "b = "; std::cin >> b;
        model = new Model36(a, b);
        modelPrefix = "Model36";
        modelTitle = "Модель 36 (Дифференциальная)";
    }
    else {
        std::cout << "Неверный выбор!\n";
        return 1;
    }

    std::cout << "\nТип входного воздействия:\n";
    std::cout << "1 - ступенчатое\n";
    std::cout << "2 - импульсное\n";
    std::cout << "3 - гармоническое\n";
    std::cout << "Ваш выбор: ";
    int signalChoice;
    std::cin >> signalChoice;

    double A;
    std::cout << "Амплитуда A = ";
    std::cin >> A;

    int n;
    std::cout << "Количество шагов n = ";
    std::cin >> n;

    std::string signalName = "";
    if (signalChoice == 1) signalName = "Step";
    else if (signalChoice == 2) signalName = "Pulse";
    else signalName = "Harmonic";

    std::string filename = modelPrefix + "_" + signalName + ".csv";
    std::ofstream fout(filename);

    fout << "tau,u,y\n";

    std::cout << "\nМодель: " << model->getName() << "\n\n";
    std::cout << std::setw(10) << "tau" << std::setw(15) << "u" << std::setw(15) << "y" << "\n";
    std::cout << "--------------------------------------------------\n";

    double dt = 0.1;

    for (int t = 0; t <= n; ++t) {
        double u = 0.0;
        if (signalChoice == 1) {
            u = A;
        }
        else if (signalChoice == 2) {
            u = (t == 0) ? A : 0.0;
        }
        else if (signalChoice == 3) {
            u = A * std::sin(t * dt);
        }

        double y_val = model->nextStep(u, dt);

        std::cout << std::setw(10) << t
            << std::setw(15) << std::fixed << std::setprecision(6) << u
            << std::setw(15) << std::fixed << std::setprecision(6) << y_val << "\n";

        fout << t << "," << u << "," << y_val << "\n";
    }

    fout.close();
    std::cout << "\nРезультаты сохранены в файл " << filename << "\n";

    std::cout << "Построение графика...\n";
    showGraph(filename, modelTitle);

    delete model;
    return 0;
}