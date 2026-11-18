#include <iostream>
#include <fstream>
#include <string>
#include <clocale>
#include "Model.h"
#include "Input.h"

template <typename InputFunc>
void testModel(Model& model, InputFunc input,
               int n, double h, const std::string& fileName)
{
    std::ofstream file(fileName);
    double y = 0;

    file << "t,u,y\n";

    for (int i = 0; i < n; i++)
    {
        double t = i * h;
        double u = input(t);

        file << t << "," << u << "," << y << "\n";

        y = model.calculate(y, u);
    }
}

int main()
{
    std::setlocale(LC_ALL, "RU");

    int n;
    std::cout << "Enter n: ";
    std::cin >> n;

    if (n <= 0)
    {
        std::cout << "n должна быть больше 0\n";
        return 1;
    }

    double a;
    double b;
    double h;
    std::cout << "Enter a: ";
    std::cin >> a;
    std::cout << "Enter b: ";
    std::cin >> b;
    std::cout << "Enter h: ";
    std::cin >> h;

    Model11 model11(a, b);
    Model25 model25(a, b);
    Model39 model39(b, h);

    testModel(model11, step, n, h, "model11_step.csv");
    testModel(model11, impulse, n, h, "model11_impulse.csv");
    testModel(model11, harmonic, n, h, "model11_harmonic.csv");

    testModel(model25, step, n, h, "model25_step.csv");
    testModel(model25, impulse, n, h, "model25_impulse.csv");
    testModel(model25, harmonic, n, h, "model25_harmonic.csv");

    testModel(model39, step, n, h, "model39_step.csv");
    testModel(model39, impulse, n, h, "model39_impulse.csv");
    testModel(model39, harmonic, n, h, "model39_harmonic.csv");

    return 0;
}