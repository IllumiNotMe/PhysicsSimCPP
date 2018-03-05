#pragma once

#include "Particle.h"
#include "Force.h"

using namespace SimEngine;

ref class LinearForce : public Force
{
public:
	LinearForce(double xMag, double yMag);
	void ApplyOn(Particle ^) override;

private:
	double xMag;
	double yMag;
};

