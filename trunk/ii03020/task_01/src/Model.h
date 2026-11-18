#pragma once

class Model {
public:
    virtual double NextStep()      = 0;
    virtual double GetY()     const = 0;
    virtual double GetLastU() const = 0;
    virtual ~Model() = default;
};