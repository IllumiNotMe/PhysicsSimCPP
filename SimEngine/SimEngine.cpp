#include "stdafx.h"
#include "SimEngine.h"

using namespace System;

SimEngine::MainEngine::MainEngine(Grid^ simGrid)
{
	graphicsHandler = gcnew GraphicsHandler(simGrid);
	particleManager = gcnew ParticleManager();
}

int SimEngine::MainEngine::Render(double deltaT)
{
	Update(deltaT);

	return NULL;
}

void SimEngine::MainEngine::Update(double deltaT)
{
	for each (Particle^ particle in particleManager->managedParticles)
	{
		ParticleLoop(particle, deltaT);
	}

	if (particleManager->managedParticles.Count >= 2)
	{
		CalcExitVelocities(particleManager->managedParticles[0], particleManager->managedParticles[1]);
	}
}

void SimEngine::MainEngine::ParticleLoop(Particle^ p, double dt)
{
	p->velY += -9.87 * dt;
	double timeLeft = dt;

	if (p->position().y + p->velocity().y * timeLeft < p->radius())
	{
		double collTime = (p->radius() - p->position().y) / p->velocity().y;
		p->TMove(collTime);
		p->velY *= -1;
		
		timeLeft -= collTime;
	}

	p->TMove(timeLeft);

}

void SimEngine::MainEngine::SpawnParticle(double x, double y, double radius, Color color)
{
	Shape2D^ particleShape = graphicsHandler->DrawCircle(x, y, radius, color);

	Particle^ particle = particleManager->SpawnParticle(x, y, radius, particleShape);
}
