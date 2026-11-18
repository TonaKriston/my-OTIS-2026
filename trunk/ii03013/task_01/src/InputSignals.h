#pragma once

#include <cstddef>

enum class InputSignalType {
    Constant,
    Impulse,
    Harmonic
};

double generateInput(
    InputSignalType type,
    std::size_t step,
    double time,
    double amplitude = 1.0,
    double omega = 1.0
);