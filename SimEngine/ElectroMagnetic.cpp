#include "stdafx.h"
#include "ElectroMagnetic.h"


using namespace System::Diagnostics;


ElectroMagnetic::ElectroMagnetic(Particle ^ particle) :
	source(particle)
{
	
}

void ElectroMagnetic::ApplyOn(Particle ^ particle)
{
	if (particle == source) return;

	// Engine length units are in decimeters - to convert to meters -> divide by 10

	double distX = (source->position().x - particle->position().x) / 10.0;
	double distY = (source->position().y - particle->position().y) / 10.0;

	double distMagSqr = pow(distX, 2) + pow(distY, 2);
	double distMag = sqrt(distMagSqr);

	double force = -1 * CoulumbConst * source->charge * particle->charge * ElementaryChargeSqr / distMagSqr;
	
	// Engine mass units are in amu - to convert to kg -> multiply by 1.661e-27
	// Multiply by 10 to convert back to decimeters from meters
	double magConst = 10000.0 * force / (distMag * particle->mass() * 1.661 * pow(10, -27));

	double accX = distX * magConst;
	double accY = distY * magConst;

	particle->accX += accX;
	particle->accY += accY;
}