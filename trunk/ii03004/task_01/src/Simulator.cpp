#include "Simulator.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

std::vector<SimulationRow> Simulator::run(Model& model,
                                          const InputSignal& signal,
                                          int n,
                                          double y0)
{
    std::vector<SimulationRow> rows;
    rows.reserve(static_cast<size_t>(n) + 1);

    model.reset(y0);
    rows.push_back({0, signal.value(0), model.currentY()});

    for (int tau = 0; tau < n; ++tau)
    {
        const double y = model.step(signal.value(tau));

        if (!std::isfinite(y) || std::fabs(y) > kDivergenceLimit)
        {
            std::cout << "ВНИМАНИЕ: процесс расходится на шаге tau = "
                      << tau + 1 << " (|y| > " << kDivergenceLimit
                      << "), моделирование остановлено.\n";
            break;
        }

        rows.push_back({tau + 1, signal.value(tau + 1), y});
    }

    return rows;
}

void Simulator::printTable(const std::vector<SimulationRow>& rows)
{
    std::cout << std::setw(6) << "tau"
              << std::setw(14) << "u"
              << std::setw(14) << "y" << '\n';
    std::cout << std::string(34, '-') << '\n';

    std::cout << std::fixed << std::setprecision(6);
    for (const SimulationRow& row : rows)
    {
        std::cout << std::setw(6) << row.tau
                  << std::setw(14) << row.u
                  << std::setw(14) << row.y << '\n';
    }
    std::cout.unsetf(std::ios::fixed);
}

bool Simulator::saveCsv(const std::vector<SimulationRow>& rows,
                        const std::string& fileName)
{
    std::ofstream file(fileName);
    if (!file)
    {
        std::cerr << "Не удалось открыть файл " << fileName << '\n';
        return false;
    }

    file << "tau,u,y\n";
    file << std::setprecision(10);
    for (const SimulationRow& row : rows)
        file << row.tau << ',' << row.u << ',' << row.y << '\n';

    return true;
}
