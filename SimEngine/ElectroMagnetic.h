#pragma once

#include "Particle.h"
#include "Force.h"

using namespace SimEngine;

ref class ElectroMagnetic : public Force
{
public:
	ElectroMagnetic(Particle ^);
	void ApplyOn(Particle ^) override;
	int fType() override { return (int)ForceType::ElectroMagnetic; }

private:
	Particle ^ source;

	static double CoulumbConst = 8.988 * pow(10, 9);
	static double ElementaryCharge = 1.602 * pow(10, -19);
	static double ElementaryChargeSqr = pow(ElementaryCharge, 2);
};

