#pragma once
#include "Model.h"

class Model2_8 : public ISystemModel {
public:
    explicit Model2_8(double a, double b, double c) : alpha(a), beta(b), gamma(c) { flushState(); }
    
    double calculateNext(double u, int t) override {
        double y = getStateHistory().empty() ? 0.1 : getStateHistory().back();
        double yPrev = getStateHistory().size() >= 2 ? getStateHistory()[getStateHistory().size()-2] : 0.1;
        double yNext = alpha*y*(1.0-y) + beta*u + gamma*std::sin(yPrev*u);
        getStateHistory().push_back(yNext);
        getControlHistory().push_back(u);
        return yNext;
    }
    
    std::string fetchName() const override { return "Model 2.8"; }
    void clearState() override { flushState(); }
    
    bool checkStability() const override {
        if (getStateHistory().size() < 10) return true;
        double mx = 0;
        for (size_t i = getStateHistory().size()-10; i < getStateHistory().size(); ++i)
            if (std::abs(getStateHistory()[i]) > mx) mx = std::abs(getStateHistory()[i]);
        return mx < 100.0;
    }
    
    void displayStabilityReport() const override {
        std::cout << "Model 2.8: non-linear, numerical check" << std::endl;
    }

private:
    void flushState() { 
        getStateHistory() = {0.1, 0.1}; 
        getControlHistory() = {0.0}; 
        getIterationIndex() = 0; 
    }
    
    double alpha;
    double beta;
    double gamma;
};