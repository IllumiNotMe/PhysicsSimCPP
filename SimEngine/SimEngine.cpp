#include "stdafx.h"
#include "SimEngine.h"

using namespace System;

SimEngine::MainEngine::MainEngine(Grid^ simGrid, TextBox^ outputConsole) :
	simGrid(simGrid)
{
	graphicsHandler = gcnew GraphicsHandler(simGrid);
	particleManager = gcnew ParticleManager();
	console = gcnew ConsoleHandler(outputConsole);

	console->WriteLine("Simulator Engine initializing...");
}

int SimEngine::MainEngine::Render(double deltaT)
{
	Update(deltaT);
	return NULL;
}

void SimEngine::MainEngine::Update(double deltaT)
{
	GridOrganization^ particleSet = gcnew GridOrganization(%particleManager->managedParticles);
	particleSet->Organize();

	for each (GEN::List<Particle ^> ^ pList in particleSet->sortedParticles.Values)
	{
		if (pList->Count >= 2)
		{
			Particle ^ p1 = pList[0];
			Particle ^ p2 = pList[1];

			double timeLeft = deltaT;
			double collTime = CalcCollisionTime(p1, p2);
			
			if (!isnan(collTime) && collTime >= 0 && collTime < timeLeft)
			{
				p1->inCollision = true;

				p1->TMove(collTime);
				p2->TMove(collTime);

				timeLeft -= collTime;

				std::pair<Vector2D, Vector2D> exitVels = CalcExitVelocities(p1, p2);
				p1->SetVelocity(exitVels.first);
				p2->SetVelocity(exitVels.second);

				p1->TMove(timeLeft);
				p2->TMove(timeLeft);
			}
		}
		
	}

	for each (Particle^ particle in particleManager->managedParticles)
	{
		// Skips normal functions if particle was in collision this frame
		if (!particle->inCollision)
		{
			ParticleLoop(particle, deltaT);
		}
		else
		{
			// Reset inCollision value
			particle->inCollision = false;
		}
	}
}

void SimEngine::MainEngine::ParticleLoop(Particle^ p, double dt)
{
	double timeLeft = dt;

	double yPosToBe = p->position().y + p->velocity().y * timeLeft;
	double xPosToBe = p->position().x + p->velocity().x * timeLeft;

	double collTime = 0;
	double newXVel = p->velX;
	double newYVel = p->velY;
	// Bottom collision
	if (yPosToBe < p->radius())
	{
		collTime = (p->radius() - p->position().y) / p->velocity().y;
		newYVel = abs(newYVel);
	}
	// Top collision
	else if (yPosToBe > simGrid->ActualHeight - p->radius())
	{
		collTime = (simGrid->ActualHeight - p->radius() - p->position().y) / p->velocity().y;
		newYVel = -1.0 * abs(newYVel);
	}
	// Left collision
	else if (xPosToBe < p->radius())
	{
		collTime = (p->radius() - p->position().x) / p->velocity().x;
		newXVel = abs(newXVel);
	}
	// Right collision
	else if (xPosToBe > simGrid->ActualWidth - p->radius())
	{
		collTime = (simGrid->ActualWidth - p->radius() - p->position().x) / p->velocity().x;
		newXVel = -1.0 * abs(newXVel);
	}
	
	if (!isinf(collTime))
	{
		p->TMove(collTime);
		p->velX = newXVel;
		p->velY = newYVel;
		timeLeft -= collTime;
	}

	p->TMove(timeLeft);

	// Check if particle is out of bounds - if so -> place in bounds
	if (p->position().y > simGrid->ActualHeight - p->radius())
	{
		p->Move(0, simGrid->ActualHeight - p->radius() - p->position().y);
	}
	if (p->position().x > simGrid->ActualWidth - p->radius())
	{
		p->Move(simGrid->ActualWidth - p->radius() - p->position().x, 0);
	}
}

// Public use spawn functions
void SimEngine::MainEngine::SpawnParticle(ParticleProperties ^ properties)
{
	Particle ^ particle = SpawnParticle(
		properties->xPos,
		properties->yPos,
		properties->xVel,
		properties->yVel,
		properties->radius,
		properties->color);
}

void SimEngine::MainEngine::SpawnParticle(double x, double y, double radius, Color color)
{
	SpawnParticle(x, y, 0, 0, radius, color);
}

// Private internal spawn
Particle ^ SimEngine::MainEngine::SpawnParticle(double x, double y, double xVel, double yVel, double radius, Color color)
{
	Shape2D^ particleShape = graphicsHandler->DrawCircle(x, y, radius, color);

	Particle^ particle = particleManager->SpawnParticle(x, y, radius, particleShape);

	particle->velX = xVel;
	particle->velY = yVel;

	return particle;
}