#pragma once
#include "Model.h"

class Model3_2 : public ISystemModel {
public:
    explicit Model3_2(double b, double dt = 0.1) : coefficientB(b), timeStep(dt) { flushState(); }
    
    double calculateNext(double u, int t) override {
        double y = getStateHistory().empty() ? 0.0 : getStateHistory().back();
        double yNext = y + timeStep * coefficientB * u;
        getStateHistory().push_back(yNext);
        getControlHistory().push_back(u);
        return yNext;
    }
    
    std::string fetchName() const override { return "Model 3.2"; }
    void clearState() override { flushState(); }
    bool checkStability() const override { return true; }
    
    void displayStabilityReport() const override {
        std::cout << "Model 3.2: integrator, stability boundary" << std::endl;
    }

private:
    void flushState() { 
        getStateHistory() = {0.0}; 
        getControlHistory() = {0.0}; 
        getIterationIndex() = 0; 
    }
    
    double coefficientB;
    double timeStep;
};