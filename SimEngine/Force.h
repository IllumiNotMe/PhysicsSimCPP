#pragma once

#include "Particle.h"

ref class Force abstract
{
public:
	Force();
	virtual void ApplyOn(Particle ^) abstract;
	
protected:
	// Force origin
	double xPos;
	double yPos;
};
