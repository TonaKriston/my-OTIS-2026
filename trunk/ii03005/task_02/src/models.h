#pragma once

#include <cstddef>
#include <string>
#include <vector>

enum class InputType
{
    Step,
    Impulse,
    Harmonic
};

class Model
{
public:
    Model(int stepCount, InputType type, double inputValue = 0.0);
    virtual ~Model() = default;

    virtual void nextStep() = 0;
    void run();
    void showResult() const;
    bool saveCsv(const std::string& outputPath) const;

    std::size_t stepCount() const { return y_.size(); }
    const std::vector<double>& inputs() const { return u_; }
    const std::vector<double>& outputs() const { return y_; }

protected:
    std::size_t currentStep() const { return current_; }
    double outputAt(std::size_t index) const;
    double inputAt(std::size_t index) const;
    void setOutput(std::size_t index, double value);
    void advanceStep() { ++current_; }
    virtual double timeAt(std::size_t index) const;

private:
    std::vector<double> y_;
    std::vector<double> u_;
    std::size_t current_{};
};

class Linear final : public Model
{
public:
    Linear(double a1, double a2, double b, std::size_t delay,
           int stepCount, InputType type, double inputValue = 0.0);
    void nextStep() override;

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
              int stepCount, InputType type, double inputValue = 0.0);
    void nextStep() override;

private:
    double a_;
    double b_;
    double e_;
    double relay(double input, double output) const;
};

class Differential final : public Model
{
public:
    Differential(double b, double timeStep,
                 int stepCount, InputType type, double inputValue = 0.0);
    void nextStep() override;

private:
    double b_;
    double dt_;
    double timeAt(std::size_t index) const override;
};
