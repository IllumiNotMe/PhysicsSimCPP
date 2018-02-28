#pragma once

using namespace System::Windows::Media;

namespace SimEngine
{
	public ref struct ParticleProperties
	{
	public:
		ParticleProperties() { color = Colors::Black; }

		// Position
		double xPos = 0;
		double yPos = 0;

		// Velocity
		double xVel = 0;
		double yVel = 0;

		// Shape
		double radius = 1;
		Color color;

		// Material properties
		double mass = 1;
	};
}