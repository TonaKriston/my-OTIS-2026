#pragma once

#include <cstddef>
#include <string>
#include <vector>

// Inputs contain u_0 ... u_tau; yPrevious is y_(tau-1).
struct SimulationState {
    std::size_t step;
    double y;
    double yPrevious;
    double u;
    const std::vector<double>& inputs;
    double dt;
};

// Abstract OOP base for each of three variant-12 models.
class Model {
public:
    virtual ~Model() = default;
    virtual std::string id() const = 0;
    virtual std::string title() const = 0;
    virtual double nextStep(const SimulationState& state) const = 0;
};

// 1.2: y_(tau+1) = a*y_tau + b*u_(tau-k), k >= 1.
class DelayedLinearModel final : public Model {
public:
    DelayedLinearModel(double a, double b, std::size_t delay);
    std::string id() const override;
    std::string title() const override;
    double nextStep(const SimulationState& state) const override;
private:
    double a_;
    double b_;
    std::size_t delay_;
};

// 2.6: y_(tau+1) = a1*y_tau^3 - a2*y_(tau-1) + b*u_tau^2.
class PolynomialNonlinearModel final : public Model {
public:
    PolynomialNonlinearModel(double a1, double a2, double b);
    std::string id() const override;
    std::string title() const override;
    double nextStep(const SimulationState& state) const override;
private:
    double a1_;
    double a2_;
    double b_;
};

// 3.10: dy/dt = -a*y + b + u; Euler with step dt.
class OffsetEulerModel final : public Model {
public:
    OffsetEulerModel(double a, double b);
    std::string id() const override;
    std::string title() const override;
    double nextStep(const SimulationState& state) const override;
private:
    double a_;
    double b_;
};
