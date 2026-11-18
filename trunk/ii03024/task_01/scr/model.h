#ifndef MODEL_H
#define MODEL_H

#include <string>

class Model {
protected:
    double y = 0.0;
public:
    virtual ~Model() {}
    virtual double nextStep(double u, double dt) = 0;
    virtual std::string getName() const = 0;
    virtual void reset() { y = 0.0; }
};

inline double deadzone(double u, double threshold = 1.0) {
    if (std::abs(u) <= threshold) return 0.0;
    return (u > 0) ? (u - threshold) : (u + threshold);
}

#endif 