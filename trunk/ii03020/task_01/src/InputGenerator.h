#pragma once

class InputGenerator {
    int type;
    int t = 0;
    double constant_u;
public:
    explicit InputGenerator(int type = 0, double constant_u = 0.0);
    double Next();
    void   Reset();
};