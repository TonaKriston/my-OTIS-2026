#pragma once

#include <string>

// Базовый класс входного воздействия u(tau).
class InputSignal
{
public:
    virtual ~InputSignal() = default;

    virtual double value(int tau) const = 0;
    virtual std::string name() const = 0;
    // Короткое имя латиницей, используется в именах файлов.
    virtual std::string id() const = 0;
};

// Ступенчатое: u(tau) = A
class StepSignal final : public InputSignal
{
public:
    explicit StepSignal(double amplitude = 1.0);
    double value(int tau) const override;
    std::string name() const override;
    std::string id() const override;

private:
    double amplitude_;
};

// Импульсное: u(0) = A, u(tau > 0) = 0  (по заданию A = 1)
class ImpulseSignal final : public InputSignal
{
public:
    explicit ImpulseSignal(double amplitude = 1.0);
    double value(int tau) const override;
    std::string name() const override;
    std::string id() const override;

private:
    double amplitude_;
};

// Гармоническое: u(tau) = A*sin(tau)
class HarmonicSignal final : public InputSignal
{
public:
    explicit HarmonicSignal(double amplitude = 1.0);
    double value(int tau) const override;
    std::string name() const override;
    std::string id() const override;

private:
    double amplitude_;
};
