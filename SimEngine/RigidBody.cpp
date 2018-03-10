#include "stdafx.h"
#include "RigidBody.h"


RigidBody::RigidBody(double xPos, double yPos, double radius, Shape2D ^ shape) :
	xPos(xPos), yPos(yPos), _radius(radius), shape(shape)
{
}
