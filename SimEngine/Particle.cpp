#include "stdafx.h"
#include "Particle.h"


SimEngine::Particle::Particle(double x, double y, double radius, double mass, Shape2D^ shape, int id) :
	x(x), y(y), _radius(radius), shape(shape), _uid(id), _mass(mass)
{
}

void SimEngine::Particle::SetPosition(double xPos, double yPos)
{
	x = xPos;
	y = yPos;
	shape->SetPosition(xPos - _radius, yPos - _radius);
}

void SimEngine::Particle::Move(double dx, double dy)
{
	SetPosition(x + dx, y + dy);
}

void SimEngine::Particle::TMove(double dt)
{
	Move(velX * dt, velY * dt);
}

void SimEngine::Particle::SetVelocity(Vector2D newVel)
{
	velX = newVel.x;
	velY = newVel.y;
}