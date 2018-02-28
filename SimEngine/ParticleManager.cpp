#include "stdafx.h"
#include "ParticleManager.h"


ParticleManager::ParticleManager()
{
	
}

Particle^ ParticleManager::SpawnParticle(double x, double y, double radius, Shape2D^ shape)
{
	Particle^ newParticle = gcnew Particle(x, y, radius, shape, currentUID);

	managedParticles.Add(newParticle);
	managagedParticles2[currentUID] = newParticle;

	currentUID += 1;

	return newParticle;
}