#pragma once
#include <string>

enum class InputType {
    Step,       
    Impulse,    
    Harmonic    
};

class Model {
private:
    double a;
    std::string name;

protected:
    double getA() const { return a; }

public:
    Model(double a_val, const std::string& model_name) : a(a_val), name(model_name) {}
    virtual ~Model() = default;

    virtual double calculateStep(double u_current, double u_prev1, double u_prev2) = 0;
    virtual void reset() = 0;

    std::string getName() const { return name; }
    double getParameterA() const { return a; }
};