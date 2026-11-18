#ifndef INPUT_SIGNAL_H
#define INPUT_SIGNAL_H

#include <string>

enum class SignalType {
    STEP,
    IMPULSE,
    HARMONIC
};

class InputSignal {
public:
    static double getSignal(SignalType type, int tau);
    static std::string getSignalName(SignalType type);
};

#endif