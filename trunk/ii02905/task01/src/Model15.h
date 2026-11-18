#pragma once
#include <vector>

class Model15 {
private:
    double y;
    double a1;
    double a2;
    double b;
    int k;
    std::vector<double> history;

public:
    Model15(double y0, double a1, double a2, double b, int k, int n);
    double step(double u, int tau);
    double getY() const;
};
