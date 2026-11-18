#include "Input.h"
#include <cmath>

double step(double)
{ 
 return 1;
}
double impulse(double t)
{
 if (t == 0) return 1;
 return 0;
}
double harmonic(double t) 
{ 
 return sin(t);
}