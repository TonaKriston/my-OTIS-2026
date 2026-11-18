#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <memory>

#include "model.h"   
#include "model1.6.h"
#include "model2.1.h"
#include "model3.7.h"

double checkSignal(int t, int type, double A) {
    switch (type) {
    case 1: return A;
    case 2: return (t == 0) ? A : 0;
    case 3: return A * std::sin(t);
    default: return 0;
    }
}

double checkdoublevalues(const std::string& text) {
    double value;
    while (true) {
        std::cout << text;
        if (std::cin >> value) return value;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Некорректный ввод, введите другое значение" << std::endl;
    }
}

int checkintvalues(const std::string& text, int minVal, int maxVal) {
    int value;
    while (true) {
        std::cout << text;
        if ((std::cin >> value) && value >= minVal && value <= maxVal) return value;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Введите целое число от " << minVal << " до " << maxVal << "!" << std::endl;
    }
}

int main(){

    std::remove("result.csv");
    std::cout << "Выберите модель:" << std::endl;
    std::cout << " 1 - Линейная модель 1.6" << std::endl;
    std::cout << " 2 - Нелинейная модель 2.1" << std::endl;
    std::cout << " 3 - Дифференциальные уравнение 3.7" << std::endl;

    int choice = checkintvalues("Вы выбрали модель: ", 1, 3);
    int number = checkintvalues("Введите количество шагов (number): ", 1, 1000);
    
    std::unique_ptr<BaseModel> model;
       switch(choice){
        case 1:{
            std::cout << "Модель 1.6: y(t+1) = a1*y(t) + a2*y(t-1) + a3*y(t-2) + b*u(t)" << std::endl;
            double a1 = checkdoublevalues("a1 = ");
            double a2 = checkdoublevalues("a2 = ");
            double a3 = checkdoublevalues("a3 = ");
            double b  = checkdoublevalues("b = ");
            model = std::make_unique<model1_6>(a1, a2, a3, b);
            break;
        }
         case 2:{
            std::cout << "Модель 2.1: y(t+1) = a*y(t) - b*y(t-1)^2 + c*u(t) + d*sin(u(t-1))" << std::endl;
            double a = checkdoublevalues("a = ");
            double b = checkdoublevalues("b = ");
            double c = checkdoublevalues("c = ");
            double d = checkdoublevalues("d = ");
            model = std::make_unique<model2_1>(a, b, c, d);
            break;
        }
        case 3:{
            std::cout << "Модель 3.7: dy/dt = -e^a*y + b*u  (y = y + h*(-e^a*y + b*u))" << std::endl;
            double a = checkdoublevalues("a = ");
            double b = checkdoublevalues("b = ");
            double h;
            while (true) {
                h = checkdoublevalues("h (шаг) = ");
                if (h > 0) break;
                std::cout << "h должно быть положительным!" << std::endl;
            }
            model = std::make_unique<model3_7>(a, b, h);
            break;
        }
        default: return 0;
    }

    std::cout << "Выберите входной сигнал u(t):" << std::endl;
    std::cout << " 1 - ступенчатое (u = const)" << std::endl;
    std::cout << " 2 - импульсное (u(0) = A, u(t > 0) = 0)" << std::endl;
    std::cout << " 3 - гармоническое (u = A * sin(t))" << std::endl;
    int inputsignal = checkintvalues("Вы выбрали сигнал: ", 1, 3);

    double ampl = checkdoublevalues("Введите амплитуду A: ");
    std::cout << std::endl;
    std::cout << std::setw(4)  << "t" << std::setw(12) << "u" << std::setw(12) << "y" << std::endl;
    std::ofstream file("result.csv");
    if (!file) {
        std::cout << "Ошибка: невозможно создать result.csv" << std::endl;
        return 1;
    }

    file << "t;u;y" << std::endl;
    file << std::fixed << std::setprecision(4);

    for (int t = 0; t < number; t++) {
        double u = checkSignal(t, inputsignal, ampl);
        double y = model->stepForward(u);
        std::cout << std::setw(4)  << t << std::setw(12) << std::fixed << std::setprecision(4) << u << std::setw(12) << y << std::endl;
        file << t << ";" << u << ";" << y << std::endl;
    }
    file.close();
    std::cout << std::endl << "Данные сохранены в result.csv" << std::endl;
    return 0;
}
