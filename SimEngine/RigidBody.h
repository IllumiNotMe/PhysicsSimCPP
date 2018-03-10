#pragma once

#include "Shape2D.h"
#include "Vector2D.h"

using namespace SimEngine;

ref class RigidBody
{
public:
	RigidBody(double xPos, double yPos, double radius, Shape2D ^ shape);

	Vector2D position() { return Vector2D(xPos, yPos); }
	double radius() { return _radius; }

private:
	double xPos;
	double yPos;

	double _radius;

	Shape2D ^ shape;
};

