#pragma once

#include <string>

// Абстрактный базовый класс математической модели.
class Model {
public:
    virtual ~Model() = default;

    virtual double next(double u) = 0;
    virtual void reset() = 0;
    virtual double value() const = 0;
    virtual std::string name() const = 0;
    virtual bool isStable() const = 0;
    virtual std::string stabilityWarning() const = 0;
};
