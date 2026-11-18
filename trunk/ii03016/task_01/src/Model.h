#pragma once

class Model {
public:
    virtual ~Model() = default;
    virtual double step(double u) = 0;
    virtual void reset() = 0;
    virtual const char* name() = 0;
};