#include "models.h"
#include "stability_analyzer.h"

#include <array>
#include <cerrno>
#include <clocale>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#include <process.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

template <typename T, typename Validator>
T readValue(const std::string& prompt,
            Validator validator,
            const std::string& validationError)
{
    T value{};
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
        {
            if (validator(value))
            {
                return value;
            }
            std::cout << validationError << '\n';
        }
        else
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please try again.\n";
        }
    }
}

template <typename T>
T readValue(const std::string& prompt)
{
    return readValue<T>(prompt, [](const T&) { return true; }, "");
}

InputType chooseInputType()
{
    const int choice = readValue<int>(
        "\n========== INPUT TYPE ==========\n"
        "1. Step\n2. Impulse\n3. Harmonic\nChoose input: ",
        [](int c) { return c >= 1 && c <= 3; },
        "Invalid choice. Try again.");

    switch (choice)
    {
        using enum InputType;
    case 1:
        return Step;
    case 2:
        return Impulse;
    default:
        return Harmonic;
    }
}

void showGraph()
{
#ifdef _WIN32
    const std::string csvPath =
        std::string(OUTPUT_DIRECTORY) + "/results.csv";
    const std::string graphPath =
        std::string(OUTPUT_DIRECTORY) + "/graph.png";
    const std::array<const char*, 5> arguments = {
        PYTHON_EXECUTABLE, PLOT_SCRIPT,
        csvPath.c_str(), graphPath.c_str(),
        nullptr
    };

    if (_spawnv(_P_WAIT, PYTHON_EXECUTABLE, arguments.data()) != 0)
    {
        std::cerr << "Не удалось запустить Python.\n";
    }
#else
    const pid_t pid = fork();

    if (pid == -1)
    {
        std::cerr << "Не удалось создать процесс Python.\n";
        return;
    }

    if (pid == 0)
    {
        const std::string csvPath =
            std::string(OUTPUT_DIRECTORY) + "/results.csv";
        const std::string graphPath =
            std::string(OUTPUT_DIRECTORY) + "/graph.png";
        execl(PYTHON_EXECUTABLE, PYTHON_EXECUTABLE, PLOT_SCRIPT,
              csvPath.c_str(), graphPath.c_str(),
              static_cast<char*>(nullptr));

        std::cerr << "Не удалось запустить Python.\n";
        _exit(EXIT_FAILURE);
    }

    int status = 0;

    while (waitpid(pid, &status, 0) == -1)
    {
        if (errno != EINTR)
        {
            std::cerr << "Ошибка ожидания завершения Python.\n";
            return;
        }
    }

    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
    {
        std::cerr << "Ошибка выполнения скрипта построения графика.\n";
    }
#endif
}

double readStepInput(InputType type)
{
    return type == InputType::Step
        ? readValue<double>("\nEnter input value: ")
        : 0.0;
}

std::unique_ptr<Model> createModel(int modelChoice, int stepCount, InputType type)
{
    const double inputValue = readStepInput(type);

    switch (modelChoice)
    {
    case 1:
    {
        std::cout << "\n\n=====================================\n";
        std::cout << "y_t+1 = a1 * y_t + a2 * y_t-k + b * u\n";
        std::cout << "=====================================\n";

        const double a1 = readValue<double>("\nEnter a1: ");
        const double a2 = readValue<double>("\nEnter a2: ");
        const double b = readValue<double>("\nEnter b: ");
        const auto delay = static_cast<std::size_t>(readValue<int>(
            "\nEnter delay k: ",
            [](int k) { return k >= 1; },
            "Delay must be at least one."));

        std::cout << "\nCharacteristic equation: z^" << delay + 1
                  << " - (" << a1 << ")*z^" << delay
                  << " - (" << a2 << ") = 0\n";
        std::cout << "Roots on the z-plane:\n";
        for (const auto& root : StabilityAnalyzer::roots(a1, a2, delay))
        {
            std::cout << "  z = " << root
                      << ", |z| = " << std::abs(root) << '\n';
        }
        if (!StabilityAnalyzer::isStable(a1, a2, delay))
        {
            std::cerr << "\nWarning: the selected linear model is unstable.\n";
            const char answer = readValue<char>(
                "The process may diverge. Continue calculation? (y/n): ",
                [](char value)
                {
                    return value == 'y' || value == 'Y'
                        || value == 'n' || value == 'N';
                },
                "Please enter y or n.");
            if (answer == 'n' || answer == 'N')
            {
                std::cout << "Calculation cancelled.\n";
                return nullptr;
            }
        }

        return std::make_unique<Linear>(
            a1, a2, b, delay, stepCount, type, inputValue);
    }
    case 2:
    {
        std::cout << "\n\n=====================================\n";
        std::cout << "y_t+1 = a * y_t + b * relay(u, y_t)\n";
        std::cout << "=====================================\n";
        return std::make_unique<NonLinear>(
            readValue<double>("\nEnter a: "),
            readValue<double>("\nEnter b: "),
            readValue<double>(
                "\nEnter threshold e: ",
                [](double e) { return e >= 0.0; },
                "Threshold must be non-negative."),
            stepCount, type, inputValue);
    }
    case 3:
    {
        std::cout << "\n\n=====================================\n";
        std::cout << "y_t+1 = y_t + dt * b * tanh(u)\n";
        std::cout << "=====================================\n";
        return std::make_unique<Differential>(
            readValue<double>("\nEnter b: "),
            readValue<double>(
                "\nEnter time step dt: ",
                [](double dt) { return dt > 0.0; },
                "Time step must be greater than zero."),
            stepCount, type, inputValue);
    }
    default:
        throw std::invalid_argument("Unknown model choice.");
    }
}

void runSelectedModel(int modelChoice)
{
    const int stepCount = readValue<int>(
        "\nEnter number of steps: ",
        [](int c) { return c >= 1; },
        "Number of steps must be greater than zero.");

    auto model = createModel(modelChoice, stepCount, chooseInputType());
    if (!model)
    {
        return;
    }
    model->run();
    std::cout << "\n------Model calculated------\n";
    model->showResult();

    if (const std::string outputPath =
            std::string(OUTPUT_DIRECTORY) + "/results.csv";
        !model->saveCsv(outputPath))
    {
        std::cerr << "Ошибка записи CSV-файла.\n";
    }
    else
    {
        std::cout << "CSV saved: " << outputPath << '\n';
    }
    showGraph();
}

int main()
{
    std::setlocale(LC_ALL, "");

    while (true)
    {
        const int choice = readValue<int>(
            "\n========== MAIN MENU ==========\n"
            "1. Linear model\n2. Nonlinear model\n"
            "3. Differential model\n0. Exit\n\nChoose model: ",
            [](int c) { return c >= 0 && c <= 3; },
            "Invalid model choice.");

        if (choice == 0)
        {
            break;
        }

        try
        {
            runSelectedModel(choice);
        }
        catch (const std::exception& error)
        {
            std::cerr << "Error: " << error.what() << '\n';
        }
    }

    return EXIT_SUCCESS;
}
