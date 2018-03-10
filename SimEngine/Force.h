#pragma once

#include "Particle.h"

using namespace SimEngine;

ref class Force abstract
{
public:
	Force();
	virtual void ApplyOn(Particle ^) abstract;

	virtual int fType() abstract;
};

public enum class ForceType
{
	Linear, ElectroMagnetic
};