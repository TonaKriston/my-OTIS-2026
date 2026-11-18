#pragma once

#include <string>

class AbstractSystem
{
public:
    virtual ~AbstractSystem() = default;

    virtual double step(double u) = 0;
    virtual void reset() = 0;
    virtual std::string getDescription() const = 0;

    virtual bool checkStability() const
    {
        return true;
    }

    virtual std::string getWarningMessage() const
    {
        return "";
    }
};