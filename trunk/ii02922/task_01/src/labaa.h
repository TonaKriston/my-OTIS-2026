#pragma once

#include <string>

class Model
{
public:
    virtual double calculateNext(double y, double u, double dt) = 0;

    virtual std::string getName() const = 0;   // <-- std:: גלוסעמ דמכמדמ string

    virtual void reset() = 0;

    virtual ~Model()
    {
    }
};