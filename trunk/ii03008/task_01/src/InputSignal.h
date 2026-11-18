#pragma once
#include "Model.h"

class InputSignal {
public:
    static double getSignal(InputType type, int tau, double amplitude);
};