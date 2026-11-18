#pragma once

#include <string>
#include <vector>

#include "InputSignal.h"
#include "Model.h"

struct SimulationRow
{
    int tau;
    double u;
    double y;
};

// Запускает моделирование и выводит результаты.
class Simulator
{
public:
    // Порог, после которого процесс считается расходящимся
    // (нужно для нелинейной модели с y^3).
    static constexpr double kDivergenceLimit = 1e6;

    // Выполнить n шагов. Результат содержит строки для tau = 0..n
    // (или меньше, если процесс разошёлся).
    static std::vector<SimulationRow> run(Model& model,
                                          const InputSignal& signal,
                                          int n,
                                          double y0);

    static void printTable(const std::vector<SimulationRow>& rows);

    static bool saveCsv(const std::vector<SimulationRow>& rows,
                        const std::string& fileName);
};
