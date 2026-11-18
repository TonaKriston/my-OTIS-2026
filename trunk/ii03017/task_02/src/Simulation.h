#pragma once

#include <vector>
#include "InputSignal.h"
#include "Model.h"

struct Row
{
    int tau;
    double u;
    double y;
};

// Runs a simulation independently of console input/output so it can be unit-tested.
std::vector<Row> simulate(const Model& model, SignalType signal,
                          double amplitude, int steps, double initialY,
                          double dt = 1.0);
