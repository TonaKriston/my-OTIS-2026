#pragma once

class Model {
public:
    virtual ~Model() = default;
    
    virtual double step(double input_signal) = 0;
    
    virtual void reset() = 0;
};