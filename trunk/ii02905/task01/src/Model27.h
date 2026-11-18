#pragma once

class Model27 {
private:
    double y;
    double a;
    double b;
    double epsilon;
    int relayState = 1;

public:
    Model27(double y0, double a, double b, double epsilon);
    double step(double u, int tau);
    double getY() const;

private:
    double relay(double u);
};
