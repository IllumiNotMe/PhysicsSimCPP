#include "stdafx.h"
#include "MathUtils.h"


std::pair<Vector2D, Vector2D> MathUtils::CalcExitVelocities(Particle^ p1, Particle^ p2)
{
	// Calculate differences in position
	double dx = p2->position().x - p1->position().x;
	double dy = p2->position().y - p1->position().y;

	// distance between both particles squared
	double distSqr = pow(dx, 2.0) + pow(dy, 2.0);

	// Check if particles are actually colliding before continuing calculations
	bool areColliding = pow(p1->radius() + p2->radius(), 2.0) > distSqr;
	if (!areColliding) return { Vector2D::NullVector(), Vector2D::NullVector() };

	// Calculate differences in velocity
	double dVelX = p1->velocity().x - p2->velocity().x;
	double dVelY = p1->velocity().y - p2->velocity().y;

	// Do some random math that will be involved in the final answer
	double dSum = dx * dVelX + dy * dVelY;
	double denominator = distSqr * (p1->mass() + p2->mass());
	double numConst = (2.0 * dSum) / denominator;
	double numConst1 = numConst * p2->mass();
	double numConst2 = numConst * p1->mass();

	// Final answers
	Vector2D p1FinalVel = Vector2D(
		p1->velocity().x - numConst1 * dx,
		p1->velocity().y - numConst1 * dy
	);
	Vector2D p2FinalVel = Vector2D(
		p2->velocity().x + numConst2 * dx,
		p2->velocity().y + numConst2 * dy
	);

	return {p1FinalVel, p2FinalVel};
}

double MathUtils::CalcCollisionTime(Particle^ p1, Particle^ p2)
{
	/*
	a = p1.x
    b = p1.y

    c = p2.x
    d = p2.y

    w = p1.velocity_x
    x = p2.velocity_x

    y = p1.velocity_y
    z = p2.velocity_y

    r = p1.r
    s = p2.r

    discrim = (2*a*w-2*a*x+2*b*y-2*b*z-2*c*w+2*c*x-2*d*y+2*d*z)**2-4*(w**2-2*w*x+x**2+y**2-2*y*z+z**2)*(a**2-2*a*c+b**2-2*b*d+c**2+d**2-r**2-2*r*s-s*2)
    # If discrim is negative or 0 -> no collision
    if discrim <= 0:
        return None

    t = (-1/2 * math.sqrt(discrim)-a*w+a*x-b*y+b*z+c*w-c*x+d*y-d*z)/(w**2-2*w*x+x**2+y**2-2*y*z+z**2)

    return t
	*/

	// Define constants for ease of use
	double a = p1->position().x;
	double b = p1->position().y;

	double c = p2->position().x;
	double d = p2->position().y;

	double w = p1->velX;
	double x = p2->velX;

	double y = p1->velY;
	double z = p2->velY;

	double r = p1->radius();
	double s = p2->radius();

	double discrim = pow(2.0*a*w - 2.0*a*x + 2.0*b*y - 2.0*b*z - 2.0*c*w + 2.0*c*x - 2.0*d*y + 2.0*d*z, 2.0) - 4.0*(pow(w, 2.0) - 2.0*w*x + pow(x, 2.0) + pow(y, 2.0) - 2.0*y*z + pow(z, 2.0))*(pow(a, 2.0) - 2.0*a*c + pow(b, 2.0) - 2.0*b*d + pow(c, 2.0) + pow(d, 2.0) - pow(r, 2.0) - 2.0*r*s - s*2.0);
	
	if (discrim <= 0)
		return nan("");
	
	double collTime = (-0.5 * sqrt(discrim) - a*w + a*x - b*y + b*z + c*w - c*x + d*y - d*z) / (pow(w, 2.0) - 2.0*w*x + pow(x, 2.0) + pow(y, 2.0) - 2.0*y*z + pow(z, 2.0));

	return collTime;
}