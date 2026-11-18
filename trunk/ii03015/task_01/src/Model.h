#pragma once

class Model
{
public:
    virtual ~Model() = default;
    virtual double calculateNext(
        double y,
        double yPrev,
        double yPrev2,
        double u,
        double dt
    ) const = 0;

    virtual const char* getName() const = 0;
};
