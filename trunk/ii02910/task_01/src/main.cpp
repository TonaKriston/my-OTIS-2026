#include <iostream>
#include <fstream>
#include <iomanip>
#include <memory>
#include <string>
#include <limits>
#include <cstdlib>

#include "AbstractSystem.h"
#include "InputGenerator.h"
#include "SystemFactory.h"

namespace
{
    int readInteger(const std::string& prompt, int lo, int hi)
    {
        while (true)
        {
            std::cout << prompt;
            if (int value; std::cin >> value && value >= lo && value <= hi)
            {
                return value;
            }
            std::cout << "Please enter a number between " << lo
                << " and " << hi << ".\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    int readSteps()
    {
        while (true)
        {
            std::cout << "Number of simulation steps (n > 0): ";
            if (int n; std::cin >> n && n > 0)
            {
                return n;
            }
            std::cout << "n must be a positive integer. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

int main()
{
    std::cout << "=============================================\n";
    std::cout << " Lab 1 | OTIS | Variant 18\n";
    std::cout << " Models: 1.8 / 2.2 / 3.6\n";
    std::cout << "=============================================\n\n";

    std::cout << "Select a model:\n";
    std::cout << "  1 - Model 1.8 (Linear ARX)\n";
    std::cout << "  2 - Model 2.2 (Actuator Saturation)\n";
    std::cout << "  3 - Model 3.6 (Cubic ODE)\n";
    int modelChoice = readInteger("Your choice: ", 1, 3);

    auto modelType = static_cast<SystemFactory::ModelType>(modelChoice);
    std::unique_ptr<AbstractSystem> system = SystemFactory::create(modelType);

    std::cout << "\nSelect input signal:\n";
    std::cout << "  1 - Constant\n";
    std::cout << "  2 - Pulse\n";
    std::cout << "  3 - Sine\n";
    int signalChoice = readInteger("Your choice: ", 1, 3);

    std::unique_ptr<InputSignal> signal;
    switch (signalChoice)
    {
    case 1: signal = std::make_unique<ConstantSignal>(1.0); break;
    case 2: signal = std::make_unique<PulseSignal>(); break;
    default: signal = std::make_unique<SineSignal>(); break;
    }

    int n = readSteps();

    std::cout << "\nModel:  " << system->getDescription() << "\n";
    std::cout << "Signal: " << signal->getLabel() << "\n";

    if (!system->checkStability())
    {
        std::cout << "\n!!! STABILITY WARNING !!!\n";
        std::cout << system->getWarningMessage() << "\n";
        std::cout << "Press Enter to continue anyway...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    std::cout << "\n";
    std::cout << std::setw(6) << "tau"
        << std::setw(14) << "u"
        << std::setw(16) << "y" << "\n";
    std::cout << std::string(38, '-') << "\n";

    std::ofstream csv("result.csv");
    csv << "tau,u_tau,y_tau\n";

    system->reset();
    for (int tau = 1; tau <= n; ++tau)
    {
        double u = signal->getValue(tau);
        double y = system->step(u);

        std::cout << std::setw(6) << tau
            << std::setw(14) << std::fixed << std::setprecision(5) << u
            << std::setw(16) << std::fixed << std::setprecision(5) << y
            << "\n";

        csv << tau << "," << u << "," << y << "\n";
    }

    csv.close();
    std::cout << std::string(38, '-') << "\n";
    std::cout << "Saved to result.csv\n";
    std::cout << "Done.\n";

#ifdef _WIN32
    std::system(
        "cd /d \"D:\\otis\\OTIS-2026\\trunk\\ii02910\\task_01\" "
        "&& python plot_results.py result.csv doc\\screenshots\\last_run.png \"Simulation result\" "
        "&& start \"\" doc\\screenshots\\last_run.png"
    );
    std::system("pause");
#endif
 
    return 0;
}