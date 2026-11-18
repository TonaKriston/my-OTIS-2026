#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <memory>
#include "InputSignal.h"
#include "Model1_8.h"
#include "Model2_10.h"
#include "Model3_2.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    #endif

    std::cout << "=========================================\n";
    std::cout << " OTIS-2026 | Лабораторная работа №1\n";
    std::cout << " Вариант 8\n";
    std::cout << "=========================================\n\n";

    std::cout << "Выберите модель:\n";
    std::cout << "1 - Model 1.8 (Generalized Autoregressive Linear Model)\n";
    std::cout << "2 - Model 2.10 (Hyperbolic Tangent Smoothing)\n";
    std::cout << "3 - Model 3.2 (Pure Constant Input Drive)\n";
    std::cout << "Ваш выбор: ";
    
    int choice = 0;
    std::cin >> choice;

    std::unique_ptr<Model> model;
    
    double a1 = 0.0;
    double a2 = 0.0;
    double b1 = 0.0;
    double b2 = 0.0;
    double a_val = 0.0;
    double b_val = 0.0;

    if (choice == 1) {
        std::cout << "\nВведите коэффициенты по одному:\n";
        std::cout << "Введите a1: "; std::cin >> a1;
        std::cout << "Введите a2: "; std::cin >> a2;
        std::cout << "Введите b1: "; std::cin >> b1;
        std::cout << "Введите b2: "; std::cin >> b2;
        model = std::make_unique<Model1_8>(a1, a2, b1, b2);
    } else if (choice == 2) {
        std::cout << "\nВведите коэффициенты по одному:\n";
        std::cout << "Введите a: "; std::cin >> a_val;
        std::cout << "Введите b: "; std::cin >> b_val;
        model = std::make_unique<Model2_10>(a_val, b_val);
    } else {
        std::cout << "\nВведите коэффициент по одному:\n";
        std::cout << "Введите b: "; std::cin >> b_val;
        model = std::make_unique<Model3_2>(b_val);
    }

    std::cout << "\nТип входного воздействия:\n";
    std::cout << "1 - ступенчатое\n";
    std::cout << "2 - импульсное\n";
    std::cout << "3 - гармоническое\n";
    std::cout << "Ваш выбор: ";
    
    int sigChoice = 0;
    std::cin >> sigChoice;
    
    InputType inputType = InputType::Step;
    if (sigChoice == 2) {
        inputType = InputType::Impulse;
    } else if (sigChoice == 3) {
        inputType = InputType::Harmonic;
    }

    double amplitude = 1.0;
    int steps = 10;
    double y0 = 0.0;

    std::cout << "\nВведите параметры моделирования:\n";
    std::cout << "Амплитуда A: "; std::cin >> amplitude;
    std::cout << "Количество шагов n: "; std::cin >> steps;
    std::cout << "Начальное значение y0: "; std::cin >> y0;

    std::cout << "\nМодель: " << model->getName() << "\n\n";

    std::cout << std::left 
              << std::setw(14) << "tau" 
              << std::setw(16) << "u" 
              << std::setw(16) << "y" << "\n";
    std::cout << "----------------------------------------------\n";

    std::vector<double> tau_vec;
    std::vector<double> u_vec;
    std::vector<double> y_vec;

    double u_prev1 = 0.0;
    double u_prev2 = 0.0;

    for (int t = 1; t <= steps; ++t) {
        double current_u = InputSignal::getSignal(inputType, t, amplitude);
        double current_y = model->calculateStep(current_u, u_prev1, u_prev2);
        
        u_prev2 = u_prev1;
        u_prev1 = current_u;

        tau_vec.push_back(t);
        u_vec.push_back(current_u);
        y_vec.push_back(current_y);

        std::cout << std::left 
                  << std::setw(14) << t 
                  << std::setw(16) << std::defaultfloat << current_u 
                  << std::setw(16) << std::defaultfloat << current_y << "\n";
    }

    std::ofstream outFile("doc/results.csv");
    outFile << "tau,u,y\n";
    for (size_t i = 0; i < tau_vec.size(); ++i) {
        outFile << tau_vec[i] << "," << u_vec[i] << "," << y_vec[i] << "\n";
    }
    outFile.close();

    std::cout << "\nРезультаты сохранены в doc/results.csv\n";
    std::cout << "Генерация графиков...\n";

    return 0;
}