#include "Simulation.h"

#include <stdexcept>

std::vector<Row> simulate(const Model& model, SignalType signal,
                          double amplitude, int steps, double initialY,
                          double dt)
{
    if (steps < 1)
        throw std::invalid_argument("steps must be at least 1");

    std::vector<Row> rows;
    rows.reserve(static_cast<std::size_t>(steps) + 1);

    double y = initialY;
    double yPrev = initialY;
    rows.push_back({0, getInput(signal, 0, amplitude), y});

    for (int tau = 0; tau < steps; ++tau)
    {
        const double u = getInput(signal, tau, amplitude);
        const double nextY = model.calculateNext(y, yPrev, u, dt);
        yPrev = y;
        y = nextY;
        rows.push_back({tau + 1, getInput(signal, tau + 1, amplitude), y});
    }
    return rows;
}
