#pragma once
#include <cmath>

inline double checkSignal(int t, int type, double A) {
    switch (type) {
    case 1: return A;
    case 2: return (t == 0) ? A : 0;
    case 3: return A * std::sin(t);
    default: return 0;
    }
}
