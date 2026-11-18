#pragma once
#include "model.hpp"
#include "signals.hpp"
#include <filesystem>

namespace otis {

void simulate(Model& model, int steps, SignalType signal,
              const std::filesystem::path& outputDirectory);
void generatePlotScript(const std::filesystem::path& resultsDirectory);

} // namespace otis
