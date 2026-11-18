#pragma once

#include <memory>
#include "AbstractSystem.h"

class SystemFactory
{
public:
    enum class ModelType
    {
        LinearARX = 1,
        Saturation = 2,
        CubicODE = 3
    };

    static std::unique_ptr<AbstractSystem> create(ModelType type);
};