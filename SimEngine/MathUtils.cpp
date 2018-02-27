#include "stdafx.h"
#include "MathUtils.h"


std::pair<Vector2D, Vector2D> MathUtils::CalcExitVelocities(Particle^ p1, Particle^ p2)
{
	/*
	def calc_exit_vels(p1, p2):
	dx = p2.x - p1.x
	dy = p2.y - p1.y

	d_sqr = dx ** 2 + dy ** 2

	# Check if particles are overlapping
	are_colliding = (p1.r + p2.r) ** 2 > d_sqr
	if not are_colliding:
	return None

	dvelx = p1.velocity_x - p2.velocity_x
	dvely = p1.velocity_y - p2.velocity_y

	dsum = dx * dvelx + dy * dvely

	denom = d_sqr * (p1.mass + p2.mass)

	c_ans = 2 * dsum / denom

	c_ans1 = c_ans * p1.mass
	vx1 = p1.velocity_x - c_ans1 * dx
	vy1 = p1.velocity_y - c_ans1 * dy

	c_ans2 = c_ans * p2.mass
	vx2 = p2.velocity_x + c_ans2 * dx
	vy2 = p2.velocity_y + c_ans2 * dy

	return [(vx1, vy1), (vx2, vy2)]
	*/

	// Calculate differences in position
	double dx = p2->position().x - p1->position().x;
	double dy = p2->position().y - p1->position().y;

	// distance between both particles squared
	double distSqr = std::pow(dx, 2.0) + std::pow(dy, 2.0);

	// Check if particles are actually colliding before continuing calculations
	bool areColliding = std::pow(p1->radius() + p2->radius(), 2.0) > distSqr;
	if (!areColliding) return {p1->velocity(), p2->velocity()};

	// Calculate differences in velocity
	double dVelX = p1->velocity().x - p2->velocity().x;
	double dVelY = p1->velocity().y - p2->velocity().y;

	// Do some random math that will be involved in the final answer
	double dSum = dx * dVelX + dy * dVelY;
	double denominator = distSqr * (p1->mass() + p2->mass());
	double numConst = 2 * dSum / denominator;
	double numConst1 = numConst * p1->mass();
	double numConst2 = numConst * p2->mass();

	// Final answers
	Vector2D p1FinalVel = Vector2D(
		p1->velocity().x - numConst1 * dx,
		p1->velocity().y - numConst1 * dy
	);
	Vector2D p2FinalVel = Vector2D(
		p2->velocity().x - numConst2 * dx,
		p2->velocity().y - numConst2 * dy
	);

	return {p1FinalVel, p2FinalVel};
}
