#pragma once

#include "Particle.h"
#include "Vector2D.h"
#include "RigidBody.h"

#include <cmath>
#include <utility>

using namespace SimEngine;

namespace MathUtils
{
	std::pair<Vector2D, Vector2D> CalcExitVelocities(Particle^ particle1, Particle^ particle2);

	Vector2D ParticleToRGColl(Particle ^, RigidBody ^);

	double CalcCollisionTime(Particle^ particle1, Particle^ particle2);
};

