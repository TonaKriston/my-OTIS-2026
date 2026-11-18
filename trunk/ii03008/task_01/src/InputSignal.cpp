#include "InputSignal.h"
#include <cmath>

double InputSignal::getSignal(InputType type, int tau, double amplitude) {
    switch (type) {
    case InputType::Step:
        return amplitude;
    case InputType::Impulse:
        return (tau == 0) ? amplitude : 0.0;
    case InputType::Harmonic:
        return amplitude * std::sin(static_cast<double>(tau));
    default:
        return 0.0;
    }
}