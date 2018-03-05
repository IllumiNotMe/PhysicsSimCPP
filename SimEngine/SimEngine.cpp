#include "stdafx.h"
#include "SimEngine.h"

using namespace System;

// Instantiation
SimEngine::MainEngine::MainEngine(Grid^ simGrid, TextBox^ outputConsole) :
	simGrid(simGrid)
{
	graphicsHandler = gcnew GraphicsHandler(simGrid);
	particleManager = gcnew ParticleManager();
	console = gcnew ConsoleHandler(outputConsole);

	console->WriteLine("Simulator Engine initializing...");
}

// Main loop
int SimEngine::MainEngine::Render(double deltaT)
{
	Update(deltaT);
	return NULL;
}
// Runs once per frame
void SimEngine::MainEngine::Update(double deltaT)
{
	for each (Particle^ particle in particleManager->managedParticles)
	{
		// Update each particle's timeLeft value
		particle->timeLeft = deltaT;

		// Update particle's velocity based on current forces
		UpdateParticle(particle);
	}

	// Organize the particles into nodes from a uniform grid
	GridOrganization^ particleSet = gcnew GridOrganization(%particleManager->managedParticles);
	particleSet->Organize();
	// Handle particle to particle collisions
	HandleCollisions(particleSet);

	// Move each particle for its remaining timeLeft
	for each (Particle^ particle in particleManager->managedParticles)
	{
		// Moves the particle based on its timeLeft and handles wall collisions
		ParticleLoop(particle);

		// Moves particle back into bounds if it left the canvas area
		FixParticlePosition(particle);
	}
}

void SimEngine::MainEngine::UpdateParticle(Particle ^ p)
{
	// Reset acceleration before calculating for this frame
	p->accX = 0;
	p->accY = 0;

	// Apply forces - Update acceleration
	for each (Force ^ force in universalForces)
	{
		force->ApplyOn(p);
	}

	// Use acceleration to update velocity
	p->velX += p->accX * p->timeLeft;
	p->velY += p->accY * p->timeLeft;
}
void SimEngine::MainEngine::HandleCollisions(GridOrganization ^ particleSet)
{
	for each (GEN::List<Particle ^> ^ pList in particleSet->sortedParticles.Values)
	{
		if (pList->Count >= 2)
		{
			Particle ^ p1 = pList[0];
			Particle ^ p2 = pList[1];

			double collTime = CalcCollisionTime(p1, p2);

			// Assumes p1.timeLeft == p2.timeLeft
			if (!isnan(collTime) && collTime >= 0 && collTime < p1->timeLeft)
			{
				p1->inCollision = true;

				p1->TMove(collTime);
				p2->TMove(collTime);

				p1->timeLeft -= collTime;
				p2->timeLeft -= collTime;

				std::pair<Vector2D, Vector2D> exitVels = CalcExitVelocities(p1, p2);
				p1->SetVelocity(exitVels.first);
				p2->SetVelocity(exitVels.second);
			}
		}
	}
}
void SimEngine::MainEngine::ParticleLoop(Particle^ p)
{
	if (p->timeLeft == 0) return;

	double yPosToBe = p->position().y + p->velocity().y * p->timeLeft;
	double xPosToBe = p->position().x + p->velocity().x * p->timeLeft;

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
		p->timeLeft -= collTime;
	}

	p->TMove(p->timeLeft);
}
void SimEngine::MainEngine::FixParticlePosition(Particle ^ p)
{
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

/*
	Public utility functions
*/
// Particles
Particle ^ SimEngine::MainEngine::SpawnParticle(ParticleProperties ^ properties)
{
	Particle ^ particle = SpawnParticle(
		properties->xPos,
		properties->yPos,
		properties->xVel,
		properties->yVel,
		properties->radius,
		properties->mass,
		properties->color);

	return particle;
}
void SimEngine::MainEngine::SpawnParticle(double x, double y, double radius, double mass, Color color)
{
	SpawnParticle(x, y, 0, 0, radius, mass, color);
}
void SimEngine::MainEngine::ClearAllParticles()
{
	while (particleManager->managedParticles.Count > 0)
	{
		Particle ^ p = particleManager->managedParticles[0];
		DeleteParticle(p);
	}
}

// Forces
void SimEngine::MainEngine::SpawnGravitationalPoint(double xPos, double yPos, double mass)
{
	universalForces.Add(gcnew GravitationPoint(xPos, yPos, mass));
}
void SimEngine::MainEngine::SpawnLinearForce(double xMag, double yMag)
{
	universalForces.Add(gcnew LinearForce(xMag, yMag));
}
void SimEngine::MainEngine::ClearAllForces()
{
	while (universalForces.Count > 0)
	{
		Force ^ force = universalForces[0];
		DeleteForce(force);
	}
}

// Private utility functions
void SimEngine::MainEngine::DeleteParticle(Particle ^ particle)
{
	graphicsHandler->RemoveShape(particle->shape);
	particleManager->DeleteParticle(particle);
}
void SimEngine::MainEngine::DeleteForce(Force ^ force)
{
	universalForces.Remove(force);
}
Particle ^ SimEngine::MainEngine::SpawnParticle(double x, double y, double xVel, double yVel, double radius, double mass, Color color)
{
	Shape2D^ particleShape = graphicsHandler->DrawCircle(x, y, radius, color);
	
	Particle^ particle = particleManager->SpawnParticle(x, y, radius, mass, particleShape);

	particle->velX = xVel;
	particle->velY = yVel;

	return particle;
}