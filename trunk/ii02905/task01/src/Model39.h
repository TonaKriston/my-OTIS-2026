#pragma once
#include <cmath>

class Model39 {
private:
    double y;
    double b;
    double dt;

public:
    Model39(double y0, double b, double dt);
    double step(double u, int tau);
    double getY() const;
};
