#pragma once

#include "Shape2D.h"
#include "Vector2D.h"

using namespace System::Windows::Shapes;

namespace SimEngine
{
	public ref class Particle
	{
	public:
		// Creation / Deletion
		Particle(double x, double y, double radius, double mass, Shape2D^, int id);

		// Position functions
		void SetPosition(double x, double y);
		void Move(double x, double y);
		void TMove(double dt);

		// Velocity
		double velX;
		double velY;
		void SetVelocity(Vector2D);

		// Acceleration
		double accX;
		double accY;

		// Chemistry
		short charge;

		// Getters
		Vector2D position() { return Vector2D(x, y); }
		Vector2D velocity() { return Vector2D(velX, velY); }
		double mass() { return _mass; }
		double radius() { return _radius; }

		Shape2D^ shape;

		// Engine
		bool inCollision = false;
		double timeLeft;

	private:
		// Position
		double x;
		double y;

		// Shape
		double _radius;

		// Momentum
		double _mass;

		// Engine
		int _uid;
	};
}


