#pragma once
#include <string>

enum class SignalType {
    STEP,
    IMPULSE,
    HARMONIC
};

class InputSignal {
public:
    static double getSignal(SignalType sig_type, int time_index);
    static std::string getSignalName(SignalType sig_type);
};