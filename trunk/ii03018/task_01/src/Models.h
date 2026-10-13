#pragma once
#include"model_standard.h"

class Model1_7 : public model_standard {
private:
	double y = 0.0;
	double u_prev1 = 0.0;
	double u_prev2 = 0.0;
	double a;
	double b1;
	double b2;
	double b3;
public:
	Model1_7(double a, double b1, double b2, double b3);
	double nextStep(double u) override;
	void reset() override;
};

class Model2_9 : public model_standard {
private:
	double y = 0.0;
	double a;
	double b;
public:
	Model2_9(double a, double b);
	double nextStep(double u) override;
	void reset() override;
};

class Model3_1 : public model_standard {
private:
	double y = 1.0;
	double a;
	double dt;
public:
	Model3_1(double a, double dt);
	double nextStep(double u) override;
	void reset() override;
};

