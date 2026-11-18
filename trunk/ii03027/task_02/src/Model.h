#pragma once
class Model {
public:
    virtual ~Model() = default;
    virtual double step(double si) = 0;
    virtual void reset() = 0;
};