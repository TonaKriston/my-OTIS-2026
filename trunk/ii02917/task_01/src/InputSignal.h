#pragma once
#include <cmath>
#include <string>

enum class SourceType { CONSTANT, DELTA, SINUSOIDAL };

class SignalGenerator {
public:
    explicit SignalGenerator(SourceType mode, double amp = 1.0, double freq = 1.0)
        : currentMode(mode), peakValue(amp), cyclesFreq(freq) {}
    
    double computeValue(int t) const {
        if (currentMode == SourceType::CONSTANT) return peakValue;
        if (currentMode == SourceType::DELTA) return (t == 0) ? peakValue : 0.0;
        return peakValue * std::sin(cyclesFreq * t);
    }
    
    std::string retrieveFormatName() const {
        if (currentMode == SourceType::CONSTANT) return "Step";
        if (currentMode == SourceType::DELTA) return "Impulse";
        return "Harmonic";
    }

private:
    SourceType currentMode;
    double peakValue;
    double cyclesFreq;
};