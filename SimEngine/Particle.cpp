#include "stdafx.h"
#include "Particle.h"


Particle::Particle(double x, double y, double radius, Shape2D^ shape) :
	x(x), y(y), _radius(radius), shape(shape)
{
}


void Particle::SetPosition(double xPos, double yPos)
{
	x = xPos;
	y = yPos;
	shape->SetPosition(xPos - _radius, yPos - _radius);
}

void Particle::Move(double dx, double dy)
{
	SetPosition(x + dx, y + dy);
}

void Particle::TMove(double dt)
{
	Move(velX * dt, velY * dt);
}

void Particle::AddVelocity(double dx, double dy)
{
	velX += dx;
	velY += dy;
}

void Particle::SetVelocity(double xVel, double yVel)
{
	velX = xVel;
	velY = yVel;
}