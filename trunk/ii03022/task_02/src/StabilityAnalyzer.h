#pragma once
#include <iostream>
#include <iomanip>
#include <cmath>

struct StabilityResult{
    double z1re;
    double z1im;  //первый корень   
    double z2re;
    double z2im;  //второй корень    
    double mod1;
    double mod2;      
    bool   isComplex;       
    bool   isStable;        
};

inline StabilityResult analyzeStability(double a1, double a2){
    StabilityResult r;
    double D = a1 * a1 + 4.0 * a2;         
    if (D >= 0){                    
        double s = std::sqrt(D);
        r.z1re = (a1 + s) / 2.0;  r.z1im = 0.0;
        r.z2re = (a1 - s) / 2.0;  r.z2im = 0.0;
        r.mod1 = std::fabs(r.z1re);
        r.mod2 = std::fabs(r.z2re);
        r.isComplex = false;
    } else {                            
        r.z1re = a1 / 2.0;
        r.z2re = a1 / 2.0;
        r.z1im =  std::sqrt(-D) / 2.0;
        r.z2im = -r.z1im;
        r.mod1 = std::sqrt(r.z1re * r.z1re + r.z1im * r.z1im);
        r.mod2 = std::sqrt(r.z1re * r.z1re + r.z1im * r.z1im);
        r.isComplex = true;
    }
    r.isStable = (r.mod1 < 1.0 && r.mod2 < 1.0);   
    return r;
}

inline bool isStable(double a1, double a2){
    return analyzeStability(a1, a2).isStable;
}

inline void printStabilityReport(double a1, double a2, std::ostream& out){
    StabilityResult r = analyzeStability(a1, a2);
    out << "Stability check: z^2 - (" << a1 << ")*z - (" << a2 << ") = 0" << std::endl;
    out << std::fixed << std::setprecision(4);
    if (!r.isComplex)
        out << "  Roots: z1 = " << r.z1re << ", z2 = " << r.z2re << std::endl;
    else
        out << "  Roots: z1 = " << r.z1re << " + j" <<  r.z1im
            << ", z2 = "    << r.z2re << " - j" << (-r.z2im) << std::endl;
    out << "  Moduli: |z1| = " << r.mod1 << ", |z2| = " << r.mod2 << std::endl;
    if (r.isStable)
        out << "  All roots are inside the unit circle -> system is STABLE." << std::endl;
    else
        out << "  WARNING: system is UNSTABLE - y(t) will diverge!" << std::endl;
}