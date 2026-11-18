#include "InputSignal.h"
#include <cmath>

double InputSignal::getSignal(SignalType type, int tau) {
    switch (type) {
        case SignalType::STEP:
            return 1.0;
        case SignalType::IMPULSE:
            return (tau == 0) ? 1.0 : 0.0;
        case SignalType::HARMONIC:
            return std::sin(0.4 * tau);
        default:
            return 0.0;
    }
}

std::string InputSignal::getSignalName(SignalType type) {
    switch (type) {
        case SignalType::STEP: return "Step";
        case SignalType::IMPULSE: return "Impulse";
        case SignalType::HARMONIC: return "Harmonic";
        default: return "Unknown";
    }
}