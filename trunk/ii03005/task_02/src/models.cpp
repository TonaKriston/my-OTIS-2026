#include "models.h"

#include <algorithm>
#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include <stdexcept>


std::size_t validateStepCount(int count)
{
    if (count < 1)
    {
        throw std::invalid_argument("Number of steps must be greater than zero.");
    }
    return static_cast<std::size_t>(count);
}

std::vector<double> makeInput(int count, InputType type, double value)
{
    auto input = std::vector(
        static_cast<std::size_t>(count), 0.0);

    switch (type){
    using enum InputType;
    case Step:
        std::ranges::fill(input, value);
        break;
    case Impulse:
        input.front() = 1.0;
        break;
    case Harmonic:
        for (std::size_t i = 0; i < input.size(); ++i)
        {
            input[i] = std::sin(static_cast<double>(i));
        }
        break;
    }
    return input;
} // GCOVR_EXCL_LINE


Model::Model(int stepCount, InputType type, double inputValue)
    : y_(validateStepCount(stepCount), 0.0),
      u_(makeInput(static_cast<int>(y_.size()), type, inputValue))
{
}

void Model::run()
{
    while (current_ + 1 < y_.size())
    {
        nextStep();
    }
}

void Model::showResult() const
{
    std::cout << "\n+----------+----------------+----------------+\n"
              << "|     t    |        u       |        y       |\n"
              << "+----------+----------------+----------------+\n";
    for (std::size_t i = 0; i < y_.size(); ++i)
    {
        std::cout << std::format("| {:>8} | {:>14.4f} | {:>14.4f} |\n",
                                  i + 1, u_[i], y_[i]);
    }
    std::cout << "+----------+----------------+----------------+\n\n";
}

bool Model::saveCsv(const std::string& outputPath) const
{
    std::ofstream file(outputPath);
    if (!file)
    {
        return false;
    }
    file << "t;u;y\n";
    for (std::size_t i = 0; i < y_.size(); ++i)
    {
        file << std::format("{:.6f};{:.6f};{:.6f}\n",
                            timeAt(i), u_[i], y_[i]);
    }
    return static_cast<bool>(file);
}

double Model::outputAt(std::size_t index) const { return y_.at(index); }
double Model::inputAt(std::size_t index) const { return u_.at(index); }
void Model::setOutput(std::size_t index, double value) { y_.at(index) = value; }
double Model::timeAt(std::size_t index) const { return static_cast<double>(index); }

Linear::Linear(double a1, double a2, double b, std::size_t delay,
               int stepCount, InputType type, double inputValue)
    : Model(stepCount, type, inputValue), a1_(a1), a2_(a2), b_(b), delay_(delay)
{
    if (delay == 0)
    {
        throw std::invalid_argument("Delay must be at least one.");
    }
}

void Linear::nextStep()
{
    const std::size_t step = currentStep();
    const double delayed = step >= delay_ ? outputAt(step - delay_) : 0.0;
    setOutput(step + 1, a1_ * outputAt(step) + a2_ * delayed + b_ * inputAt(step));
    advanceStep();
}

NonLinear::NonLinear(double a, double b, double e,
                     int stepCount, InputType type, double inputValue)
    : Model(stepCount, type, inputValue), a_(a), b_(b), e_(e)
{
    if (e < 0.0)
    {
        throw std::invalid_argument("Threshold must be non-negative.");
    }
}

void NonLinear::nextStep()
{
    const std::size_t step = currentStep();
    const double currentY = outputAt(step);
    setOutput(step + 1, a_ * currentY + b_ * relay(inputAt(step), currentY));
    advanceStep();
}

double NonLinear::relay(double input, double output) const
{
    if (input > e_ || (input >= -e_ && output > 0.0))
    {
        return 1.0;
    }
    if (input < -e_ || (input <= e_ && output <= 0.0))
    {
        return -1.0;
    }
    return 0.0; // GCOVR_EXCL_LINE
}

Differential::Differential(double b, double timeStep,
                           int stepCount, InputType type, double inputValue)
    : Model(stepCount, type, inputValue), b_(b), dt_(timeStep)
{
    if (timeStep <= 0.0)
    {
        throw std::invalid_argument("Time step dt must be greater than zero.");
    }
}

void Differential::nextStep()
{
    const std::size_t step = currentStep();
    setOutput(step + 1, outputAt(step) + dt_ * b_ * std::tanh(inputAt(step)));
    advanceStep();
}

double Differential::timeAt(std::size_t index) const
{
    return static_cast<double>(index) * dt_;
}
