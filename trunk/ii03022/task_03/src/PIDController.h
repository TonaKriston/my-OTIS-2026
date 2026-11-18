#pragma once

class PIDController{
    private:
        double Kp;
        double Ki;
        double Kd;       // коэффициенты регулятора
        double dt;               // шаг квантования
        double uMin;
        double uMax;       // физические пределы актуатора
        double u_prev  = 0;      // u(t-1)
        double e_prev1 = 0;      // e(t-1)
        double e_prev2 = 0;      // e(t-2)
    public:
        PIDController(double Kp, double Ki, double Kd, double dt, double uMin, double uMax)
        : Kp(Kp), Ki(Ki), Kd(Kd), dt(dt), uMin(uMin), uMax(uMax)
        {
        }

        double step(double w, double y){
            double e = w - y;
            double dU = Kp * (e - e_prev1)
                      + Ki * e * dt
                      + (Kd / dt) * (e - 2.0 * e_prev1 + e_prev2);
            double u = u_prev + dU;
            if (u > uMax) u = uMax;
            else if (u < uMin) u = uMin;
            e_prev2 = e_prev1;
            e_prev1 = e;
            u_prev = u;
            return u;
        }

        void reset(){
            u_prev = 0;
            e_prev1 = 0;
            e_prev2 = 0;
        }
};
