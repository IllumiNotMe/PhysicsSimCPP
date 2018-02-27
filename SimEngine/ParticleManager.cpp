#include "stdafx.h"
#include "ParticleManager.h"


ParticleManager::ParticleManager()
{
	
}


ParticleManager::~ParticleManager()
{

}

Particle^ ParticleManager::SpawnParticle(double x, double y, double radius, Shape2D^ shape)
{
	Particle^ newParticle = gcnew Particle(x, y, radius, shape);

	managedParticles.Add(newParticle);

	return newParticle;
}