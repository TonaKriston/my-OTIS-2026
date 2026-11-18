#include <cmath>
#include "model.h"

model1_1::model1_1(double a, double b) : a(a), b(b) {}

double model1_1::step(double y_tau, double u_tau) {
	return a * y_tau + b * u_tau;
}

double model2_3::deadzone(double u) const {
	if (u > delta) {
		return u - delta;
	}
	if (u < -delta) {
		return u + delta;
	}
	return 0.0;
}

model2_3::model2_3(double a, double b, double delta) : a(a), b(b), delta(delta) {}

double model2_3::step(double y_tau, double u_tau) {
	return a * y_tau + b * deadzone(u_tau);
}

model3_5::model3_5(double b, double dt) : b(b), dt(dt) {}

double model3_5::step(double y_tau, double u_tau) {
	return y_tau + dt * b * std::sin(u_tau);
}
