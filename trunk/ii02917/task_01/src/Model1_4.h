#pragma once
#include "Model.h"

class Model1_4 : public ISystemModel {
public:
    Model1_4(double a, double b) : gainA(a), gainB(b) { flushState(); }
    
    double calculateNext(double u, int t) override {
        double y = getStateHistory().empty() ? 0.0 : getStateHistory().back();
        double uPrev = getControlHistory().size() >= 2 ? getControlHistory()[getControlHistory().size()-2] : 0.0;
        double yNext = gainA * y + gainB * (u - uPrev);
        getStateHistory().push_back(yNext);
        getControlHistory().push_back(u);
        return yNext;
    }
    
    std::string fetchName() const override { return "Model 1.4"; }
    void clearState() override { flushState(); }
    
    bool checkStability() const override { return std::abs(gainA) < 1.0; }
    
    void displayStabilityReport() const override {
        std::cout << "Model 1.4: z=" << gainA << " |z|=" << std::abs(gainA);
        std::cout << (checkStability() ? " STABLE" : " UNSTABLE") << std::endl;
    }

private:
    void flushState() { 
        getStateHistory() = {0.0}; 
        getControlHistory() = {0.0}; 
        getIterationIndex() = 0; 
    }
    
    double gainA;
    double gainB;
};