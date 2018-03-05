#include "stdafx.h"
#include "ParticleManager.h"

ParticleManager::ParticleManager()
{
	
}

void ParticleManager::DeleteParticle(Particle ^ particle)
{
	managedParticles.Remove(particle);
}

Particle^ ParticleManager::SpawnParticle(double x, double y, double radius, double mass, Shape2D^ shape)
{
	Particle^ newParticle = gcnew Particle(x, y, radius, mass, shape, currentUID);

	managedParticles.Add(newParticle);

	currentUID += 1;

	return newParticle;
}