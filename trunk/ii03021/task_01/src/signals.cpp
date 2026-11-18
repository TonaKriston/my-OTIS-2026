#include "signals.hpp"
#include <cmath>

namespace otis {

double inputSignal(SignalType type, int tau) {
    switch (type) {
    case SignalType::Step: return 1.0;
    case SignalType::Impulse: return tau == 0 ? 1.0 : 0.0;
    case SignalType::Harmonic: return std::sin(static_cast<double>(tau));
    }
    return 0.0;
}

const char* signalName(SignalType type) {
    switch (type) {
    case SignalType::Step: return "step";
    case SignalType::Impulse: return "impulse";
    case SignalType::Harmonic: return "harmonic";
    }
    return "unknown";
}

} // namespace otis
