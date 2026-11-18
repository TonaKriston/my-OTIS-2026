
#pragma once

enum class SignalType : int
{
    Step = 1,
    Impulse,
    Harmonic
};

double getInput(const SignalType signal, const int time, const double amplitude);
