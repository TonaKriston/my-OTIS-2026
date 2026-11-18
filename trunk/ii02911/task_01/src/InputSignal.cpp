#include "InputSignal.h"
#include <cmath>

double getInput(SignalType type, int tau, double amplitude)
{
    if (type == SignalType::Step)
        return amplitude;

    if (type == SignalType::Impulse)
    {
        if (tau != 0)
            return 0.0;

        return amplitude;
    }

    if (type == SignalType::Harmonic)
    {
        auto time = static_cast<double>(tau);
        auto value = std::sin(time);
        return amplitude * value;
    }

    return 0.0;
}
