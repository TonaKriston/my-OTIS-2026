#pragma once

namespace otis {

enum class SignalType { Step, Impulse, Harmonic };

double inputSignal(SignalType type, int tau);
const char* signalName(SignalType type);

} // namespace otis
