#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <filesystem>
#include <system_error>

#include "Linear_Model.h"
#include "Non_Linear_Model.h"
#include "Simple_Differential_Equation.h"
#include "TableOutput.h"
#include "Plotter.h"

#ifdef _WIN32
  #include <windows.h>
#endif

using namespace std;

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    int input_type;
    int steps;
    char plot_choice;
    int model_type;
    double u_const = 0.0;

    cout << "Выберите входное воздействие:\n"
         << "  0 - ступенчатое (const)\n"
         << "  1 - импульсное (u0=1, uτ=0 при τ>0)\n"
         << "  2 - гармоническое sin(τ)\n"
         << "Ваш выбор: ";
    cin >> input_type;
    cout << endl;

    if (input_type == 0) {
        cout << "Введите постоянное значение u: ";
        cin >> u_const;
        cout << endl;
    }

    cout << "Введите количество шагов симуляции: ";
    cin >> steps;
    if (steps <= 0) {
        cerr << "Ошибка: количество шагов должно быть > 0\n";
        return 1;
    }
    cout << endl;

    cout << "Запустить gnuplot для построения графиков?\n"
         << "  Y - да (нужен gnuplot в PATH)\n"
         << "  N - только сохранить .plt-скрипты\n"
         << "Ваш выбор: ";
    cin >> plot_choice;
    cout << endl;
    bool invoke = (plot_choice != 'N' || plot_choice != 'n');

    if (invoke && !Plotter::is_gnuplot_available()) {
        cerr << "Внимание: gnuplot не найден в PATH. "
                "Будут сохранены только .plt-скрипты.\n";
        invoke = false;
    }

    double a;
    double b;
    double c;
    double d;

    double b1;
    double b2;
    double b3;

    bool ok = true;

    cout << "Выберите тип модели:\n"
         << "  0 - Линейная\n"
         << "  1 - Нелинейная\n"
         << "  2 - Дифференциальное\n"
         << "Ваш выбор: ";
    cin >> model_type;
    cout << endl;

    switch(model_type){
    case 0:
    {
        cout << "Введите значение параметра a: ";
        cin >> a;
         cout << "Введите значение параметра b1: ";
        cin >> b1;
        cout << "Введите значение параметра b2: ";
        cin >> b2;
        cout << "Введите значение параметра b3: ";
        cin >> b3;

        Linear_Model m(a, b1, b2, b3, input_type, u_const);
        ok = run_and_report(m, steps, "Линейная модель", "linear_model.csv") && ok;
        Plotter::plot_csv("linear_model.csv",
                          "linear_model.plt",
                          "linear_model.png",
                          "Linear model", invoke);

        cout << "\nГотово. Файл результата:\n"
         << "  linear_model.png\n";
        
        if(plot_choice == 'Y' || plot_choice == 'y')
            system("start \"\" linear_model.png");  
        break;
    }
    case 1:
    {
        cout << "Введите значение параметра a: ";
        cin >> a;
        cout << "Введите значение параметра b: ";
        cin >> b;
        cout << "Введите значение параметра c: ";
        cin >> c;
        cout << "Введите значение параметра d: ";
        cin >> d;

        Non_linear_Model m(a, b, c, d, input_type, u_const);
        ok = run_and_report(m, steps, "Нелинейная модель", "nonlinear_model.csv") && ok;
        Plotter::plot_csv("nonlinear_model.csv",
                          "nonlinear_model.plt",
                          "nonlinear_model.png",
                          "Nonlinear model", invoke);
            
        cout << "\nГотово. Файл результата:\n"
         << "  nonlinear_model.png\n";

        if(plot_choice == 'Y' || plot_choice == 'y')
            system("start \"\" nonlinear_model.png");
        break;
    }
    case 2:
    {
        cout << "Введите значение параметра b: ";
        cin >> b;

        Simple_Differential_Equation m(b, 0.1, input_type, u_const);
        ok = run_and_report(m, steps, "Дифференциальное уравнение", "sde_model.csv") && ok;
        Plotter::plot_csv("sde_model.csv",
                          "sde_model.plt",
                          "sde_model.png",
                          "Differential equation", invoke);
        cout << "\nГотово. Файл результата:\n"
         << "  sde_model.png\n";
        
        if(plot_choice == 'Y' || plot_choice == 'y')
            system("start \"\" sde_model.png");
        break;
    }
    default: break;
    }

    char type;

    cout << endl;
    cout << "Нужно удалять временные файлы?\n"
         << "  Y - да\n"
         << "  N - нет\n"
         << "Ваш выбор: ";
    cin >> type;
    cout << endl;

    if(type == 'Y' || type == 'y')
    {
    cout << "\nОчистка временных файлов...\n";

    // Список файлов для удаления
    std::vector<std::string> files_to_delete = {
        "linear_model.csv", "nonlinear_model.csv", "sde_model.csv",
        "linear_model.plt", "nonlinear_model.plt", "sde_model.plt", "comparison.plt"
    };

    for (const auto& file : files_to_delete) {
        std::error_code ec;
        // Пытаемся удалить файл
        std::filesystem::remove(file, ec);
    }
    }

    return ok ? 0 : 1;
}