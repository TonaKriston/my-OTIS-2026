#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <locale>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>
#endif

#include "model.h"
#include "linear_model_1_9.h"
#include "nonlinear_model_2_9.h"
#include "diff_eq_3_9.h"

void removeOldFiles() {
    std::remove("out_linear_result.csv");
    std::remove("out_nonlinear_result.csv");
    std::remove("out_diffeq_result.csv");
}

void simulate(Model* model, const std::string& filename, int n, int signal_type) {
    std::ofstream csv(filename);
    if (!csv.is_open()) {
        std::cerr << "Ошибка: не удалось создать файл " << filename << "\n";
        return;
    }

    csv << "step,u,y\n";
    model->reset();

    std::cout << "\n===== " << filename << " =====\n";
    std::cout << std::left
              << std::setw(8)  << "step"
              << std::setw(14) << "u"
              << std::setw(14) << "y" << "\n";
    std::cout << std::string(36, '-') << "\n";

    for (int t = 0; t < n; ++t) {
        double u = 0.0;
        if (signal_type == 0)      u = 1.0;
        else if (signal_type == 1) u = (t == 0) ? 1.0 : 0.0;
        else if (signal_type == 2) u = std::sin(0.5 * t);

        double y = model->step(u);
        csv << t << "," << u << "," << y << "\n";

        std::cout << std::left
                  << std::setw(8)  << t
                  << std::setw(14) << u
                  << std::setw(14) << y << "\n";
    }

    std::cout << std::string(36, '-') << "\n";
    csv.close();
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
   
    std::setlocale(LC_NUMERIC, "C");

    int choice;
    std::string prefix;

    std::cout << " ВЫБЕРИТЕ МОДЕЛЬ \n";
    std::cout << "1 - Линейная модель 1.9\n";
    std::cout << "2 - Нелинейная модель 2.9\n";
    std::cout << "3 - Дифференциальное уравнение 3.9\n";
    std::cout << "Ваш выбор (1/2/3): ";
    std::cin >> choice;

    int n;
    std::cout << "Введите количество шагов симуляции n: ";
    std::cin >> n;
    if (n <= 0) {
        std::cerr << "Ошибка: n должно быть положительным\n";
        return 1;
    }

    int signal_type;
    std::cout << "Выберите тип входного сигнала:\n";
    std::cout << "0 - Ступенчатый (u = 1)\n";
    std::cout << "1 - Импульсный (u = 1 при t = 0)\n";
    std::cout << "2 - Гармонический (u = sin(0.5*t))\n";
    std::cout << "Ваш выбор (0/1/2): ";
    std::cin >> signal_type;
    if (signal_type < 0 || signal_type > 2) {
        std::cerr << "Ошибка: signal_type должен быть 0, 1 или 2\n";
        return 1;
    }

    removeOldFiles();

    if (choice == 1) {
       
        double a1;
        double a2;
        double a3;
        double b1;
        
        std::cout << "Введите коэффициент a1: ";
        std::cin >> a1;
        std::cout << "Введите коэффициент a2: ";
        std::cin >> a2;
        std::cout << "Введите коэффициент a3: ";
        std::cin >> a3;
        std::cout << "Введите коэффициент b1: ";
        std::cin >> b1;
        
        LinearModel1_9 m(a1, a2, a3, b1);
        prefix = "out_linear";
        simulate(&m, prefix + "_result.csv", n, signal_type);
    }
    else if (choice == 2) {
       
        double a;
        double b;
        
        std::cout << "Введите коэффициент a: ";
        std::cin >> a;
        std::cout << "Введите коэффициент b: ";
        std::cin >> b;
        
        NonlinearModel2_9 m(a, b);
        prefix = "out_nonlinear";
        simulate(&m, prefix + "_result.csv", n, signal_type);
    }
    else if (choice == 3) {
    
        double b;
        double dt;
        
        std::cout << "Введите коэффициент b: ";
        std::cin >> b;
        std::cout << "Введите коэффициент dt: ";
        std::cin >> dt;
        
        DiffEq3_9 m(b, dt);
        prefix = "out_diffeq";
        simulate(&m, prefix + "_result.csv", n, signal_type);
    }
    else {
        std::cout << "Неверный выбор!\n";
        return 1;
    }

    std::string cmd = "python plot.py " + prefix + " " + std::to_string(signal_type);
    system(cmd.c_str());

    return 0;
}