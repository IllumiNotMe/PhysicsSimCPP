#pragma once

#include "Particle.h"
#include "Shape2D.h"

using namespace System::Windows::Shapes;
using namespace System::Collections::Generic;

ref class ParticleManager
{
public:
	ParticleManager();

	Particle^ SpawnParticle(double x, double y, double radius, double mass, Shape2D^);
	void DeleteParticle(Particle ^);

	System::Collections::Generic::List<Particle^> managedParticles;

private:
	int currentUID = 0;
};

