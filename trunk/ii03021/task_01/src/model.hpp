#pragma once

namespace otis {

class Model {
public:
    virtual ~Model() = default;
    virtual double step(double input, int tau) = 0;
    virtual void reset() = 0;
    [[nodiscard]] virtual const char* name() const = 0;
};

} // namespace otis
