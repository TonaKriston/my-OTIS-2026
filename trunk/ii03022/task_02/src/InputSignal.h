#pragma once 
#include <cmath>

inline double inputSignal(int t, int type, double ampl){
    switch (type){
    case 1: return ampl;
    case 2: return (t == 0) ? ampl : 0;
    case 3: return ampl * std::sin(t);
    default: return 0;
    }
}