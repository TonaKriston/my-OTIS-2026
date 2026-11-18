#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <cmath>

enum class InputType {
    STEP,
    IMPULSE,
    HARMONIC
};

class Model {
private:
    double y_prev1 = 0.0;
    double y_prev2 = 0.0;
    double y_prev3 = 0.0;

protected:
    double getPrev1() const { return y_prev1; }
    double getPrev2() const { return y_prev2; }
    double getPrev3() const { return y_prev3; }
    
    void setPrev(double p1, double p2, double p3) {
        y_prev1 = p1;
        y_prev2 = p2;
        y_prev3 = p3;
    }

public:
    // Исправлено: инициализация через список инициализации
    explicit Model(double initial_y) : y_prev1(initial_y) {}

    virtual ~Model() = default;

    virtual double calculateNext(double u, int step) = 0;

    virtual std::vector<double> simulate(int steps, InputType type) {
        std::vector<double> results;
        results.push_back(y_prev1);
        
        for (int i = 1; i < steps; ++i) {
            double u = getSignal(type, i);
            double next_y = calculateNext(u, i);
            results.push_back(next_y);
        }
        return results;
    }

    virtual void reset(double initial_y) {
        y_prev1 = initial_y;
        y_prev2 = 0.0;
        y_prev3 = 0.0;
    }

    static double getSignal(InputType type, int tau) {
        if (type == InputType::STEP) {
            return 1.0;
        }
        if (type == InputType::IMPULSE) {
            return (tau == 1) ? 1.0 : 0.0;
        }
        if (type == InputType::HARMONIC) {
            return std::sin(static_cast<double>(tau));
        }
        return 0.0;
    }
};

#endif // MODEL_H