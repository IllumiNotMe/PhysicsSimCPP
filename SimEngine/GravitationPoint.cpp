#include "stdafx.h"
#include "GravitationPoint.h"


GravitationPoint::GravitationPoint(double xPosition, double yPosition, double mass) :
	_mass(mass)
{
	xPos = xPosition;
	yPos = yPosition;
}

void GravitationPoint::ApplyOn(Particle ^ particle)
{
	double distX = (xPos - particle->position().x);
	double distY = (yPos - particle->position().y);

	double distMagSqr = pow(distX, 2) + pow(distY, 2);
	double distMag = sqrt(distMagSqr);

	double magAcceleration = UnivGravConst * _mass / distMagSqr;

	double magAccX = distX * magAcceleration / distMag;
	double magAccY = distY * magAcceleration / distMag;

	particle->accX += magAccX;
	particle->accY += magAccY;
}