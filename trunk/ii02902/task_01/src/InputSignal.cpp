#include "InputSignal.h"
#include <cmath>
#include <array>

double InputSignal::getSignal(SignalType sig_type, int time_index) {
    if (sig_type == SignalType::STEP) {
        return 1.0;
    }
    if (sig_type == SignalType::IMPULSE) {
        return time_index == 0 ? 1.0 : 0.0;
    }
    if (sig_type == SignalType::HARMONIC) {
        constexpr double freq_multiplier = 0.4;
        return std::sin(freq_multiplier * static_cast<double>(time_index));
    }
    return 0.0;
}

std::string InputSignal::getSignalName(SignalType sig_type) {
    static constexpr std::array<const char*, 3> names = {
        "Step",
        "Impulse",
        "Harmonic"
    };

    // Объявили idx прямо внутри условия if (C++17 init-statement) для SonarCloud (cpp:S6004)
    if (const auto idx = static_cast<size_t>(sig_type); idx < names.size()) {
        return names[idx];
    }
    return "Unknown";
}