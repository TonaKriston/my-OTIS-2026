#pragma once
#include "model.h"

class model1_6 : public BaseModel{
    private:
        double a1;
        double a2;
        double a3;
        double b;
        double y = 0;          
        double y_prev = 0;   
        double y_prev_prev = 0;  
    public:
        model1_6(double a1, double a2, double a3, double b)
        : a1(a1), a2(a2), a3(a3),b(b)
        {
        }

        double stepForward(double u) override{
            double y_next = (a1 * y) + (a2 * y_prev) + (a3 * y_prev_prev) + (b * u);
            y_prev_prev = y_prev;
            y_prev = y;
            y = y_next;
            return y_next;
        }

        void clearState() override{
            y = 0;  
            y_prev = 0;
            y_prev_prev = 0;
        };
};