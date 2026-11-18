
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <memory>
#include <limits>
#include <cmath>

#include "models.h"
#include "Model1_10.h"
#include "Model2_2.h"
#include "Model3_4.h"
#include "InputSignals.h"

using namespace std;

// Ввод целого числа с проверкой
int readInt(const string& message, int minValue, int maxValue)
{
    int value;

    while (true)
    {
        cout << message;

        if (cin >> value && value >= minValue && value <= maxValue)
        {
            return value;
        }

        cout << "Incorrect input. Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Ввод действительного числа с проверкой
double readDouble(const string& message)
{
    double value;

    while (true)
    {
        cout << message;

        if (cin >> value && isfinite(value))
        {
            return value;
        }

        cout << "Incorrect number. Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Моделирование одной выбранной модели
vector<double> simulate(
    const Model& model,
    const vector<double>& u,
    double initialY,
    double dt)
{
    vector<double> y(u.size(), 0.0);

    if (u.empty())
    {
        return y;
    }

    y[0] = initialY;

    for (size_t t = 0; t + 1 < u.size(); ++t)
    {
        double delayedU = (t >= 2) ? u[t - 2] : 0.0;

        y[t + 1] = model.next(
            y[t],
            u[t],
            delayedU,
            dt
        );
    }

    return y;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "=================================\n";
    cout << "       MODELING PROGRAM\n";
    cout << "=================================\n";

    // 1. Выбор модели
    cout << "\nChoose a model:\n";
    cout << "1 - Model1_10 (linear)\n";
    cout << "2 - Model2_2 (saturation)\n";
    cout << "3 - Model3_4 (differential)\n";

    int modelChoice = readInt("Your choice: ", 1, 3);

    // 2. Ввод коэффициентов и создание модели
    unique_ptr<Model> model;
    double initialY = 0.0;

    if (modelChoice == 1)
    {
        cout << "\nModel1_10 coefficients:\n";

        double a = readDouble("Enter a: ");
        double b = readDouble("Enter b: ");
        double c = readDouble("Enter c: ");

        model = make_unique<Model1_10>(a, b, c);
    }
    else if (modelChoice == 2)
    {
        cout << "\nModel2_2 coefficients:\n";

        double a = readDouble("Enter a: ");
        double b = readDouble("Enter b: ");
        double minValue = readDouble("Enter lower saturation limit: ");
        double maxValue = readDouble("Enter upper saturation limit: ");

        while (minValue > maxValue)
        {
            cout << "Lower limit must not exceed upper limit.\n";
            minValue = readDouble("Enter lower saturation limit: ");
            maxValue = readDouble("Enter upper saturation limit: ");
        }

        model = make_unique<Model2_2>(
            a, b, minValue, maxValue
        );
    }
    else
    {
        cout << "\nModel3_4 coefficients:\n";

        double coefficient = readDouble("Enter coefficient: ");

        model = make_unique<Model3_4>(coefficient);

        initialY = 2.0;
    }

    // 3. Параметры моделирования
    const double dt = 0.1;

    int steps = readInt(
        "\nEnter number of steps (1-100000): ",
        1, 100000
    );

    double amplitude = readDouble("Enter input amplitude: ");

    // 4. Выбор входного сигнала
    cout << "\nChoose input signal:\n";
    cout << "1 - Constant\n";
    cout << "2 - Impulse\n";
    cout << "3 - Harmonic\n";

    int signalChoice = readInt("Your choice: ", 1, 3);

    double omega = 1.0;

    if (signalChoice == 3)
    {
        omega = readDouble("Enter angular frequency: ");

        while (omega <= 0.0)
        {
            cout << "Frequency must be greater than zero.\n";
            omega = readDouble("Enter angular frequency: ");
        }
    }

    InputSignalType signalType;

    switch (signalChoice)
    {
    case 1:
        signalType = InputSignalType::Constant;
        break;
    case 2:
        signalType = InputSignalType::Impulse;
        break;
    default:
        signalType = InputSignalType::Harmonic;
        break;
    }

    // 5. Формирование входного сигнала
    vector<double> u(steps + 1);

    for (int t = 0; t <= steps; ++t)
    {
        u[t] = generateInput(
            signalType,
            t,
            t * dt,
            amplitude,
            omega
        );
    }

    // 6. Расчёт выбранной модели
    vector<double> y = simulate(
        *model,
        u,
        initialY,
        dt
    );

    // 7. Сохранение результатов в CSV
    ofstream file("results.csv");

    if (!file.is_open())
    {
        cout << "Cannot create results.csv\n";
        return 1;
    }

    file << "step,time,input,model_output\n";
    file << fixed << setprecision(6);

    for (int t = 0; t <= steps; ++t)
    {
        file << t << ','
             << t * dt << ','
             << u[t] << ','
             << y[t] << '\n';
    }

    file.close();

    // 8. Вывод результатов
    cout << "\nResults for selected model: "
         << model->name() << "\n\n";

    cout << setw(8) << "step"
         << setw(12) << "time"
         << setw(12) << "input"
         << setw(16) << "output" << '\n';

    cout << fixed << setprecision(4);

    int rowsToPrint = (steps < 10) ? steps : 10;

    for (int t = 0; t <= rowsToPrint; ++t)
    {
        cout << setw(8) << t
             << setw(12) << t * dt
             << setw(12) << u[t]
             << setw(16) << y[t] << '\n';
    }

    cout << "\nResults saved to results.csv\n";

    return 0;
}
