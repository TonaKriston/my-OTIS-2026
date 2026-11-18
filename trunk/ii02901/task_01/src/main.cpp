#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>
#include <fstream>
#include <memory>
#include "model.h"

double inputType(int tau, int type) {
	switch (type) {
	case 1: return 1.0;
	case 2: return (tau == 0) ? 1.0 : 0.0;
	case 3: return std::sin(tau);
	default: return 0.0;
	}
}

int main() {
	
	unsigned int n;
	std::cout << "Enter number of simulation steps: \n";
	if (!(std::cin >> n)) {
		std::cerr << "Invalid input for number of steps.\n";
		return 1;
	}

	unsigned int m;
	std::cout << "Select Model: (1 - Model 1_1, 2 - Model 2_3, 3 - Model 3_5)\n";
	if (!(std::cin >> m)) {
		std::cerr << "Invalid model choice.\n";
		return 1;
	}

	std::unique_ptr<model> selectedModel;
	std::string modelName;

	switch (m) {
	case 1:
		
		double a1;
		double b1;
		std::cout << "\n- Model 1.1 (First-Order Inertial System) -\n";
		std::cout << "Enter a1, b1: ";
		if (!(std::cin >> a1 >> b1)) {
			std::cerr << "Invalid input for model 1.1 parameters.\n";
			return 1;
		}
		selectedModel = std::make_unique<model1_1>(a1, b1);
		modelName = "Model 1.1";
		break;

	case 2:
		
		double a2;
		double b2;
		double delta;
		std::cout << "\n- Model 2.3 (Dead-Zone Non-linearity) -\n";
		std::cout << "Enter a2, b2, delta: ";
		if (!(std::cin >> a2 >> b2 >> delta)) {
			std::cerr << "Invalid input for model 2.3 parameters.\n";
			return 1;
		}
		selectedModel = std::make_unique<model2_3>(a2, b2, delta);
		modelName = "Model 2.3";
		break;

	case 3:

		double b3;
		double dt;
		std::cout << "\n- Model 3.5 (Harmonic Driving Force) -\n";
		std::cout << "Enter b3, dt: ";
		if (!(std::cin >> b3 >> dt)) {
			std::cerr << "Invalid input for model 3.5 parameters.\n";
			return 1;
		}
		selectedModel = std::make_unique<model3_5>(b3, dt);
		modelName = "Model 3.5";
		break;
		
	default:
		return 1;
	}

	unsigned int in;
	std::cout << "Select Input Type: (1 - Step, 2 - Impulse, 3 - Harmonic)\n";
	if (!(std::cin >> in)) {
		std::cerr << "Invalid input type choice.\n";
		return 1;
	}

	std::string inputName;

	switch (in) {
	case 1: inputName = "Step"; break;
	case 2: inputName = "Impulse"; break;
	case 3: inputName = "Harmonic"; break;
	default: return 1;
	}

	std::cout << "\n" << std::setw(6) << "tau"
		<< " | " << std::setw(12) << "u(tau)"
		<< " | " << std::setw(12) << "Input"
		<< " | " << std::setw(12) << "y(tau)" << "\n";
	constexpr int LINE_WIDTH = 50;
	std::cout << std::string(LINE_WIDTH, '-') << "\n";

	std::ofstream csv_file("results.csv");
	if (!csv_file.is_open()) {
		std::cerr << "Failed to open results.csv for writing.\n";
		return 1;
	}

	csv_file << "tau;u(tau);Input;y(tau)\n";

	double y = 0.0;

	for (unsigned int tau = 0; tau < n; ++tau) {
		double u = inputType(static_cast<int>(tau), in);
		y = selectedModel->step(y, u);

		std::cout << std::setw(6) << tau
			<< " | " << std::setw(12) << std::fixed << std::setprecision(4) << u
			<< " | " << std::setw(12) << inputName
			<< " | " << std::setw(12) << y << "\n";

		csv_file << tau << ";" << u << ";" << inputName << ";" << y << "\n";
	}

	return 0;
}
