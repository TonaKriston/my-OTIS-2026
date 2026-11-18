#pragma once
#include <fstream>
#include "../../task_01/src/model.h"

double generateSignal(int signalChoice, double amplitude, int tau);
void runSimulation(Model& model, std::ofstream& csvFile);
