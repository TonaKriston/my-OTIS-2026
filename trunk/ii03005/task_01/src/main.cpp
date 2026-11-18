#include <array>
#include <cmath>
#include <clocale>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <cerrno>

#ifdef _WIN32
#include <process.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif


namespace fs = std::filesystem;

enum class InputType
{
    Step,
    Impulse,
    Harmonic
};

class Model
{
public:
    Model(int stepCount, InputType type, double inputValue = 0.0)
        : stepCount_(validateStepCount(stepCount)),
          inputType_(type)
    {
        y_.assign(stepCount_, 0.0);
        countU(inputValue);
    }

    virtual ~Model() = default;
    virtual void nextStep() = 0;

    void run()
    {
        while (current_ + 1 < stepCount_)
        {
            nextStep();
        }
    }

    void showResult() const
    {
        std::cout << '\n'
                  << "+----------+----------------+----------------+\n"
                  << "|     t    |        u       |        y       |\n"
                  << "+----------+----------------+----------------+\n";

        for (std::size_t i = 0; i < stepCount_; ++i)
        {
            std::cout << std::format(
                "| {:>8} | {:>14.4f} | {:>14.4f} |\n",
                (i+1), u_[i], y_[i]);
        }

        std::cout << "+----------+----------------+----------------+\n\n";
    }

    void saveCsv()
    {
        const fs::path outputPath = fs::path(OUTPUT_DIRECTORY) / "results.csv";
        std::ofstream file(outputPath);

        if (!file.is_open())
        {
            std::cerr << "Не удалось открыть файл.\n";
            return;
        }

        file << "t;u;y\n";
        for (std::size_t i = 0; i < stepCount_; ++i)
        {
            file << std::format("{:.6f};{:.6f};{:.6f}\n",
                                timeAt(i), u_[i], y_[i]);
        }

        if (!file)
        {
            std::cerr << "Ошибка записи CSV-файла.\n";
            return;
        }

        filename_ = outputPath.string();
        std::cout << "CSV saved: " << filename_ << '\n';
    }

protected:
    std::size_t currentStep() const { return current_; }
    double outputAt(std::size_t index) const { return y_.at(index); }
    double inputAt(std::size_t index) const { return u_.at(index); }
    void setOutput(std::size_t index, double value) { y_.at(index) = value; }
    void advanceStep() { ++current_; }

private:
    static std::size_t validateStepCount(int stepCount)
    {
        if (stepCount < 1)
        {
            throw std::invalid_argument("Number of steps must be greater than zero.");
        }
        return static_cast<std::size_t>(stepCount);
    }

    void countU(double inputValue)
    {
        switch (inputType_)
        {
            using enum InputType;
        case Step:
            u_.assign(stepCount_, inputValue);
            break;
        case Impulse:
            u_.assign(stepCount_, 0.0);
            u_.front() = 1.0;
            break;
        case Harmonic:
            u_.resize(stepCount_);
            for (std::size_t i = 0; i < stepCount_; ++i)
            {
                u_[i] = std::sin(static_cast<double>(i));
            }
            break;
        }
    }

    std::size_t stepCount_{};
    std::size_t current_{};
    InputType inputType_;
    std::vector<double> y_{};
    std::vector<double> u_{};
    std::string filename_{};

protected:
    virtual double timeAt(std::size_t index) const
    {
        return static_cast<double>(index);
    }
};

class Linear final : public Model
{
public:
    Linear(double a1, double a2, double b, std::size_t delay,
           int stepCount, InputType type, double inputValue = 0.0)
        : Model(stepCount, type, inputValue),
          a1_(a1), a2_(a2), b_(b), delay_(delay)
    {
    }

    void nextStep() override
    {
        const std::size_t step = currentStep();
        double delayedY = 0.0;
        if (step >= delay_)
        {
            delayedY = outputAt(step - delay_);
        }

        setOutput(step + 1,
                  a1_ * outputAt(step) + a2_ * delayedY + b_ * inputAt(step));
        advanceStep();
    }

private:
    double a1_;
    double a2_;
    double b_;
    std::size_t delay_;
};

class NonLinear final : public Model
{
public:
    NonLinear(double a, double b, double e,
              int stepCount, InputType type, double inputValue = 0.0)
        : Model(stepCount, type, inputValue),
          a_(a), b_(b), e_(e)
    {
    }

    void nextStep() override
    {
        const std::size_t step = currentStep();
        const double currentY = outputAt(step);
        setOutput(step + 1, a_ * currentY + b_ * relay(inputAt(step), currentY));
        advanceStep();
    }

private:
    double a_;
    double b_;
    double e_;

    double relay(double input, double output) const
    {
        if (input > e_ || (input >= -e_ && output > 0.0))
        {
            return 1.0;
        }
        if (input < -e_ || (input <= e_ && output <= 0.0))
        {
            return -1.0;
        }
        return 0.0;
    }
};

class Differential final : public Model
{
public:
    Differential(double b, double timeStep,
                 int stepCount, InputType type, double inputValue = 0.0)
        : Model(stepCount, type, inputValue),
          b_(b),
          dt_(validateTimeStep(timeStep))
    {
    }

    void nextStep() override
    {
        const std::size_t step = currentStep();
        setOutput(step + 1,
                  outputAt(step) + dt_ * b_ * std::tanh(inputAt(step)));
        advanceStep();
    }

private:
    double b_;
    double dt_;

    static double validateTimeStep(double dt)
    {
        if (dt <= 0.0)
        {
            throw std::invalid_argument("Time step dt must be greater than zero.");
        }
        return dt;
    }

    double timeAt(std::size_t index) const override
    {
        return static_cast<double>(index) * dt_;
    }
};

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
    const std::array<const char*, 3> arguments = {
        PYTHON_EXECUTABLE, PLOT_SCRIPT, nullptr
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
        execl(PYTHON_EXECUTABLE, PYTHON_EXECUTABLE, PLOT_SCRIPT,
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
        return std::make_unique<Linear>(
            readValue<double>("\nEnter a1: "),
            readValue<double>("\nEnter a2: "),
            readValue<double>("\nEnter b: "),
            static_cast<std::size_t>(readValue<int>(
                "\nEnter delay k: ",
                [](int k) { return k >= 1; },
                "Delay must be at least one.")),
            stepCount, type, inputValue);
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
    model->run();
    std::cout << "\n------Model calculated------\n";
    model->showResult();
    model->saveCsv();
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

        runSelectedModel(choice);
    }

    return EXIT_SUCCESS;
}