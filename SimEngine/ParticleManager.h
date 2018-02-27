#pragma once

#include "Particle.h"
#include "Shape2D.h"
#include <forward_list>

using namespace System::Windows::Shapes;
using namespace System::Collections::Generic;

ref class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	Particle^ SpawnParticle(double x, double y, double radius, Shape2D^);
	
	List<Particle^> managedParticles;
};

