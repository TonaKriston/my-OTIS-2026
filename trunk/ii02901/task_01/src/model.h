#pragma once

class model {
public:

	virtual ~model() = default;
	virtual double step(double y_tau, double u_tau) = 0;

};

class model1_1 : public model {
private:

	double a;
	double b;

public:

	model1_1(double a, double b);
	double step(double y_tau, double u_tau) override;

};

class model2_3 : public model {
private:

	double a;
	double b;
	double delta;

	double deadzone(double u) const;

public:

	model2_3(double a, double b, double delta);
	double step(double y_tau, double u_tau) override;

};

class model3_5 : public model {
private:

	double b;
	double dt;

public:

	model3_5(double b, double dt);
	double step(double y_tau, double u_tau) override;

};