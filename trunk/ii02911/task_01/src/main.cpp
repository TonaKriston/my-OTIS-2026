
#include <iostream>
#include <fstream>
#include <iomanip>
#include <memory>
#include <cmath>
#include <cstdlib>
#include <clocale>

#include "Model.h"
#include "Model1_9.h"
#include "Model2_6.h"
#include "Model3_9.h"
#include "InputSignal.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "");

    int choice;

    cout << "=== Mathematical Model Simulation ===\n";
    cout << "1. Model 1.9\n";
    cout << "2. Model 2.6\n";
    cout << "3. Model 3.9\n";
    cout << "Choose model (1-3): ";

    if (!(cin >> choice)) {
        cout << "Error: please enter a number.\n";
        return 1;
    }

    if (choice < 1 || choice > 3) {
        cout << "Error: model number must be from 1 to 3.\n";
        return 1;
    }

    unique_ptr<Model> model;

    double a1 = 0.0;
    double a2 = 0.0;
    double a3 = 0.0;
    double b1 = 0.0;
    double b = 0.0;
    double dt = 0.1;

    switch (choice) {
    case 1:
        cout << "Enter a1, a2, a3, b1: ";

        if (!(cin >> a1 >> a2 >> a3 >> b1)) {
            cout << "Error: invalid model coefficients.\n";
            return 1;
        }

        model = make_unique<Model1_9>(a1, a2, a3, b1);
        break;

    case 2:
        cout << "Enter a1, a2, b: ";

        if (!(cin >> a1 >> a2 >> b)) {
            cout << "Error: invalid model coefficients.\n";
            return 1;
        }

        model = make_unique<Model2_6>(a1, a2, b);
        break;

    case 3:
        cout << "Enter b: ";

        if (!(cin >> b)) {
            cout << "Error: invalid coefficient.\n";
            return 1;
        }

        cout << "Enter time step dt: ";

        if (!(cin >> dt) || !isfinite(dt) || dt <= 0.0) {
            cout << "Error: dt must be a positive finite number.\n";
            return 1;
        }

        model = make_unique<Model3_9>(b);
        break;

    default:
        cout << "Error: unknown model.\n";
        return 1;
    }

    int signalChoice;
    double amplitude;
    int steps;

    cout << "\nInput signal types:\n";
    cout << "1. Step\n";
    cout << "2. Impulse\n";
    cout << "3. Harmonic\n";
    cout << "Choose input signal (1-3): ";

    if (!(cin >> signalChoice) ||
        signalChoice < 1 || signalChoice > 3) {
        cout << "Error: signal type must be from 1 to 3.\n";
        return 1;
    }

    auto signalType =
        static_cast<SignalType>(signalChoice);

    cout << "Enter signal amplitude: ";

    if (!(cin >> amplitude) || !isfinite(amplitude)) {
        cout << "Error: amplitude must be a finite number.\n";
        return 1;
    }

    cout << "Enter number of simulation steps: ";

    if (!(cin >> steps) || steps <= 0) {
        cout << "Error: number of steps must be positive.\n";
        return 1;
    }

    ofstream file("result.csv");

    if (!file.is_open()) {
        cout << "Error: could not create result.csv.\n";
        return 1;
    }

    file << "Step,Input,Output\n";

    double y = 0.0;
    double yPrev = 0.0;
    double yPrev2 = 0.0;

    cout << "\nModel: " << model->getName() << "\n";
    cout << fixed << setprecision(4);
    cout << setw(10) << "Step"
         << setw(15) << "Input"
         << setw(15) << "Output" << "\n";

    for (int tau = 0; tau < steps; ++tau) {
        double u = getInput(signalType, tau, amplitude);

        double nextY =
            model->calculateNext(y, yPrev, yPrev2, u, dt);

        if (!isfinite(nextY)) {
            cout << "Error: model produced an invalid result.\n";
            file.close();
            return 1;
        }

        cout << setw(10) << tau
             << setw(15) << u
             << setw(15) << nextY << "\n";

        file << tau << "," << u << "," << nextY << "\n";

        yPrev2 = yPrev;
        yPrev = y;
        y = nextY;
    }

    file.close();

    cout << "\nSimulation completed successfully.\n";
    cout << "Results saved to result.csv.\n";
    cout << "Generating plot...\n";

    if (auto plotResult = system("python plot.py"); plotResult != 0) {
    cout << "Could not generate the plot.\n";
    cout << "Check Python, matplotlib and the location of plot.py.\n";
}

    return 0;
}
