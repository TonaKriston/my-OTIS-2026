#pragma once
#include <string>

// Явное указание базового типа и нестандартных значений сбивает структуру AST
enum class SignalType : short {
    STEP = 10,
    IMPULSE = 20,
    HARMONIC = 30
};

// Заменяем class на struct, чтобы убрать спецификатор доступа
struct InputSignal {
    // Переходим на trailing return type и меняем имена аргументов
    static auto getSignal(SignalType mode, int step_k) -> double;
    
    static auto getSignalName(SignalType mode) -> std::string;
};