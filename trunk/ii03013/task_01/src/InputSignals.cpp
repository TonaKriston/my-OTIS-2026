#include "InputSignals.h"
#include <cmath>

double generateInput(
    InputSignalType type,
    std::size_t step,
    double time,
    double amplitude,
    double omega
) {
    switch (type) {
    case InputSignalType::Constant:
        return amplitude;

    case InputSignalType::Impulse:
        return (step == 0) ? amplitude : 0.0;

    case InputSignalType::Harmonic:
        return amplitude * std::sin(omega * time);
    }

    return 0.0;
}