#include "stdafx.h"
#include "LinearForce.h"


LinearForce::LinearForce(double xMag, double yMag) :
	xMag(xMag), yMag(yMag)
{
}

void LinearForce::ApplyOn(Particle ^ p)
{
	p->accX += xMag;
	p->accY += yMag;
}