#pragma once
#include <iosfwd>
#include <string>
#include <vector>
#include "Simulation.h"

// Small, deterministic application helpers separated from interactive main().
bool requiresIntegrationStep(const char* modelName);
bool isValidSignalChoice(int choice);
bool isValidModelChoice(int choice);
void printResultsTable(std::ostream& out, const std::vector<Row>& rows);
bool saveResultsCsv(const std::string& path, const std::vector<Row>& rows);

// Анализ устойчивости Model 1.3: печатает корни и сообщение (предупреждение,
// если модель неустойчива). Возвращает true для асимптотически устойчивой модели.
bool printStabilityReport(std::ostream& out, double a1, double a2);
