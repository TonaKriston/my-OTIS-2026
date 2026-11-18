#pragma once
#include <string>
#include <vector>
#include <iosfwd>

#include "Model.h"

// Одна строка таблицы результатов
struct SimRow {
    int    step;
    double y;
    double u;
    bool   u_valid;   // на шаге 0 u ещё не потреблялось
};

// Прогоняет модель на заданное число шагов и собирает результаты
std::vector<SimRow> simulate(Model& m, int steps);

// Выводит таблицу в произвольный поток (консоль / файл)
void write_table(const std::vector<SimRow>& rows, std::ostream& out,
                 char sep = ',');

// Сохраняет таблицу в CSV-файл; возвращает false при ошибке открытия
bool save_table_to_file(const std::vector<SimRow>& rows,
                        const std::string& filename);

// Высокоуровневая обёртка: прогнать → вывести на консоль → сохранить в файл
bool run_and_report(Model& m, int steps,
                    const std::string& title,
                    const std::string& filename);