#pragma once

#include "Particle.h"
#include "Shape2D.h"

using namespace System::Windows::Shapes;
using namespace System::Collections::Generic;

ref class ParticleManager
{
public:
	ParticleManager();

	Particle^ SpawnParticle(double x, double y, double radius, Shape2D^);
	
	System::Collections::Generic::List<Particle^> managedParticles;

	Dictionary<int, Particle^> managagedParticles2;

private:
	int currentUID = 0;
};

