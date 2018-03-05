#pragma once

#include "Particle.h"
#include "Force.h"

using namespace SimEngine;

ref class GravitationPoint : public Force
{
public:
	GravitationPoint(double, double, double);
	void ApplyOn(Particle ^) override;

private:
	static double UnivGravConst = 6.673 * pow(10, -11);
	double _mass;
};

