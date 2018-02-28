#pragma once

#include "Shape2D.h"
#include "Vector2D.h"

using namespace System::Windows::Shapes;

ref class Particle
{
public:
	Particle(double x, double y, double radius, Shape2D^, int id);

	// Position functions
	void SetPosition(double x, double y);
	void Move(double x, double y);
	void TMove(double dt);

	// Velocity
	void SetVelocity(double x, double y);
	void AddVelocity(double dx, double dy);

	double velX;
	double velY;

	// Velocity functions
	void SetVelocity(Vector2D);

	// Getters
	Vector2D position() { return Vector2D(x, y); }
	Vector2D velocity() { return Vector2D(velX, velY); }
	double mass() { return _mass; }
	double radius() { return _radius; }

	// Engine
	bool inCollision = false;

private:
	// Position
	double x;
	double y;

	// Shape
	double _radius;
	Shape2D^ shape;

	// Momentum
	double _mass = 1;

	// Engine
	int _uid;
};

