#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

#include "Model15.h"
#include "Model27.h"
#include "Model39.h"

using namespace std;

enum class InputType {
    Constant = 1,
    Impulse = 2,
    Harmonic = 3
};

double getInput(InputType type, int tau) {
    if (type == InputType::Constant)
        return 1.0;

    if (type == InputType::Impulse)
        return (tau == 0) ? 1.0 : 0.0;

    return sin(static_cast<double>(tau));
}

template <typename ModelType>
vector<double> simulate(ModelType& model, int n, InputType inputType) {
    vector<double> result;

    result.push_back(model.getY());

    for (int tau = 0; tau < n; ++tau) {
        double u = getInput(inputType, tau);
        result.push_back(model.step(u, tau));
    }

    return result;
}

void printTable(const string& title, const vector<double>& values) {
    cout << "\n" << title << "\n";
    cout << "-------------------------\n";
    cout << "tau\t y\n";

    for (size_t i = 0; i < values.size(); ++i) {
        cout << i << "\t"
             << fixed << setprecision(6)
             << values[i] << "\n";
    }
}

void saveCSV(const string& filename, const vector<double>& values) {
    ofstream file(filename);

    file << "tau,y\n";

    for (size_t i = 0; i < values.size(); ++i) {
        file << i << ","
            << fixed << setprecision(6)
            << values[i] << "\n";
    }
}

string inputName(InputType type) {
    if (type == InputType::Constant)
        return "constant";

    if (type == InputType::Impulse)
        return "impulse";

    return "harmonic";
}

int main() {
    setlocale(LC_ALL, "RU");
    cout << "Ëàáîðàòîðíàÿ ðàáîòà ¹1 - Âàðèíàò 5\n";
    cout << "Models: 1.5, 2.7, 3.9\n\n";

    int n;

    cout << "Ââåäèòå êîë-âî âåðøèí n: ";
    cin >> n;

    if (n <= 0) {
        cout << "Îøèáêà! Êîë-âî âåðøèí ìåíüøå 0\n";
        return 1;
    }

    // Ìîäåëü 1.5 
    const double a1 = 0.6;
    const double a2 = 0.2;
    const double b15 = 0.5;
    const int k = 2;

    // Ìîäåëü 2.7 
    const double a27 = 0.8;
    const double b27 = 0.7;
    const double epsilon = 0.2;

    // Ìîäåëü 3.9 
    const double b39 = 1.0;
    const double dt = 0.1;

    for (int input = 1; input <= 3; ++input) {
        auto type = static_cast<InputType>(input);
        string name = inputName(type);

        Model15 model15(0.0, a1, a2, b15, k, n);
        Model27 model27(0.0, a27, b27, epsilon);
        Model39 model39(0.0, b39, dt);

        vector<double> result15 = simulate(model15, n, type);
        vector<double> result27 = simulate(model27, n, type);
        vector<double> result39 = simulate(model39, n, type);

        printTable("Model 1.5 - " + name, result15);
        printTable("Model 2.7 - " + name, result27);
        printTable("Model 3.9 - " + name, result39);

        saveCSV("model15_" + name + ".csv", result15);
        saveCSV("model27_" + name + ".csv", result27);
        saveCSV("model39_" + name + ".csv", result39);
    }

    return 0;
}
