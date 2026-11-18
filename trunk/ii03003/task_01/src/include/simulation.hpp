#pragma once

#include "models.hpp"

#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

enum class InputType { Step, Impulse, Harmonic };

std::string inputName(InputType type);
double inputAt(InputType type, std::size_t step);

struct Row {
    std::size_t tau;
    double input;
    double yCurrent;
    double yNext;
};

std::vector<Row> simulate(const Model& model, InputType signal, std::size_t n,
                          double dt, double y0 = 0.0);

void writeCsv(const std::filesystem::path& path, const std::vector<Row>& rows);

// Create a standalone vector chart (opens in a browser, no Python required).
void writeSvg(const std::filesystem::path& path, const Model& model,
              const std::vector<Row>& stepRows,
              const std::vector<Row>& impulseRows,
              const std::vector<Row>& harmonicRows);
