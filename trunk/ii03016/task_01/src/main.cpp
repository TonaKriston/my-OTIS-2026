#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "Model.h"
#include "Model 1.10.h"
#include "Model 2.5.h"
#include "Model 3.10.h"

// Линейная модель (вариант 1.10) 
LineModel_1_10::LineModel_1_10(double a1_, double b1_, double b2_)
    : a1(a1_), b1(b1_), b2(b2_) {}

void LineModel_1_10::reset() {
    y = 0.0;
    u_prev1 = 0.0;
    u_prev2 = 0.0;
}

double LineModel_1_10::step(double u) {
    double y_new = a1 * y + b1 * u + b2 * u_prev2;
    u_prev2 = u_prev1;
    u_prev1 = u;
    y = y_new;
    return y;
}

const char* LineModel_1_10::name() {
    return "Linear Model 1.10";
}

// Нелинейная модель (вариант 2.5) 
NonlinearModel_2_5::NonlinearModel_2_5(double a_, double b_)
    : a(a_), b(b_) {}

void NonlinearModel_2_5::reset() {
    y = 0.0;
}

double sign(double x) {
    if (x > 0) return 1.0;
    if (x < 0) return -1.0;
    return 0.0;
}

double NonlinearModel_2_5::step(double u) {
    y = a * y + b * sign(u) * (1.0 - std::exp(-std::fabs(u)));
    return y;
}

const char* NonlinearModel_2_5::name() {
    return "Nonlinear Model 2.5";
}

// Дифференциальное уравнение(выриант 3.10)
Diff_3_10::Diff_3_10(double a_, double b_, double dt_)
    : a(a_), b(b_), dt(dt_) {}

void Diff_3_10::reset() {
    y = 0.0;
}

double Diff_3_10::step(double u) {
    y = y + dt * (-a * y + b + u);
    return y;
}

const char* Diff_3_10::name() {
    return "Diff 3.10";
}

// Сигнал
double generate_u(int tau, int signal_type, double A) {
    switch (signal_type) {
        case 1: return A;                          // ступенчатый
        case 2: return (tau == 0) ? A : 0.0;       // импульсный
        case 3: return A * std::sin(tau);          // гармонический
        default: return 0.0;
    }
}

const char* signal_name(int type) {
    switch (type) {
        case 1: return "step";
        case 2: return "impulse";
        case 3: return "harmonic";
        default: return "unknown";
    }
}

// Симуляция
void simulate(Model& model, int n_steps, int signal_type, double A, std::ofstream& file) {
    std::cout << "\n";
    std::cout << std::string(45, '=') << "\n";
    std::cout << "  " << model.name() << "\n";
    std::cout << "  Signal: " << signal_name(signal_type) << " (A = " << A << ")\n";
    std::cout << std::string(45, '=') << "\n";

    std::cout << std::setw(6)  << "t"
              << std::setw(14) << "u(t)"
              << std::setw(16) << "y(t)" << std::endl;
    std::cout << std::string(36, '-') << std::endl;

    file << "Model," << model.name() << ",Signal," << signal_name(signal_type) << "\n";
    file << "t,u(t),y(t)\n";

    model.reset();
    for (int tau = 0; tau <= n_steps; ++tau) {
        double u = generate_u(tau, signal_type, A);
        double y = model.step(u);

        std::cout << std::setw(6)  << tau
                  << std::setw(14) << std::fixed << std::setprecision(4) << u
                  << std::setw(16) << std::fixed << std::setprecision(4) << y
                  << std::endl;

        file << tau << "," << u << "," << y << "\n";
    }
    file << "\n";
    std::cout << std::string(36, '-') << std::endl;
}


int main() {
    int choice;
    int n_steps;
    int signal_type;
    double A;

    std::cout << "Choose model:\n";
    std::cout << " 1 - Model 1.10\n";
    std::cout << " 2 - Model 2.5\n";
    std::cout << " 3 - Model 3.10\n";
    std::cout << "Your choice: ";
    std::cin >> choice;

    if (choice < 1 || choice > 3) {
        std::cerr << "Ошибка: неверный выбор модели.\n";
        return 1;
    }

    std::cout << "Number of steps (n): ";
    std::cin >> n_steps;
    if (n_steps <= 0) {
        std::cerr << "Ошибка: n должно быть > 0.\n";
        return 1;
    }

    std::cout << "\n";
    if (choice == 1) {
        std::cout << "Model 1.10: y(t+1) = a1*y(t) + b1*u(t) + b2*u(t-2)\n";
        std::cout << "a1 = 0.8\n";
        std::cout << "b1 = 1.0\n";
        std::cout << "b2 = 0.5\n";
    } else if (choice == 2) {
        std::cout << "Model 2.5: y(t+1) = a*y(t) + b*sign(u(t))*(1 - e^(-|u(t)|))\n";
        std::cout << "a = 0.7\n";
        std::cout << "b = 0.5\n";
    } else {
        std::cout << "Model 3.10: dy/dt = -a*y + b + u\n";
        std::cout << "a  = 0.5\n";
        std::cout << "b  = 0.2\n";
        std::cout << "dt = 0.1\n";
    }

    std::cout << "\nChoose input signal u(t):\n";
    std::cout << " 1 - step      (u = A)\n";
    std::cout << " 2 - impulse   (u(0) = A, u(t > 0) = 0)\n";
    std::cout << " 3 - harmonic  (u = A * sin(t))\n";
    std::cout << "Your choice: ";
    std::cin >> signal_type;

    if (signal_type < 1 || signal_type > 3) {
        std::cerr << "Ошибка: неверный выбор сигнала.\n";
        return 1;
    }

    
    std::cout << "Input amplitude A: ";
    std::cin >> A;

   std::ofstream file("results.csv", std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть results.csv\n";
        return 1;
    }

    if (choice == 1) {
        LineModel_1_10 model(0.8, 1.0, 0.5);
        simulate(model, n_steps, signal_type, A, file);
    } else if (choice == 2) {
        NonlinearModel_2_5 model(0.7, 0.5);
        simulate(model, n_steps, signal_type, A, file);
    } else {
        Diff_3_10 model(0.5, 0.2, 0.1);
        simulate(model, n_steps, signal_type, A, file);
    }

    file.close();
    std::cout << "\nData saved to results.csv\n";
    return 0;
}
