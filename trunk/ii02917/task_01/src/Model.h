#pragma once

#include <cmath>
#include <string>
#include <iostream>
#include <vector>

class ISystemModel {
public:
    virtual ~ISystemModel() = default;
    virtual double calculateNext(double inputValue, int stepIndex) = 0;
    virtual std::string fetchName() const = 0;
    virtual void clearState() = 0;
    virtual bool checkStability() const { return true; }
    virtual void displayStabilityReport() const {
        std::cout << "  Stability analysis not applicable for this model." << std::endl;
    }

protected:
    // Защищенные методы доступа для дочерних классов
    std::vector<double>& getStateHistory() { return stateHistory_; }
    const std::vector<double>& getStateHistory() const { return stateHistory_; }

    std::vector<double>& getControlHistory() { return controlHistory_; }
    const std::vector<double>& getControlHistory() const { return controlHistory_; }

    int& getIterationIndex() { return iterationIndex_; }
    const int& getIterationIndex() const { return iterationIndex_; }

private:
    std::vector<double> stateHistory_;
    std::vector<double> controlHistory_;
    int iterationIndex_ = 0;
};