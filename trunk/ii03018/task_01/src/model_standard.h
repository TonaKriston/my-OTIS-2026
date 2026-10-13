#pragma once

class model_standard {
public:
	virtual ~model_standard() = default;
	virtual double nextStep(double u) = 0;
	virtual void reset() = 0;
};