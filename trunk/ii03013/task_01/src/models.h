#pragma once

#include <string>

class Model {
public:
    virtual ~Model() = default;

    virtual double next(
        double y,
        double u,
        double delayedU,
        double dt
    ) const = 0;

    virtual void reset() {}

    virtual std::string name() const = 0;

    virtual bool isStable() const {
        return true;
    }

    virtual std::string stabilityWarning() const {
        return "";
    }
};