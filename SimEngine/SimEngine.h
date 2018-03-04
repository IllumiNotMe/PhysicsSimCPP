// SimEngine.h

#pragma once

#include <chrono>
#include <ctime>
#include <cmath>
#include <utility>

#include "GraphicsHandler.h"
#include "ParticleManager.h"
#include "ConsoleHandler.h"
#include "GridOrganization.h"
#include "Shape2D.h"
#include "MathUtils.h"
#include "ParticleProperties.h"
#include "GravitationPoint.h"
#include "LinearForce.h"

using namespace System::Windows::Controls;
using namespace System::Windows::Shapes;
using namespace System::Windows::Media;
using namespace System::Windows;

using namespace System::Diagnostics;

using namespace System::Collections::Generic;
namespace GEN = System::Collections::Generic;

using namespace std::chrono;

using namespace MathUtils;

namespace SimEngine {

	public ref class MainEngine
	{
	public:
		// Instantiation
		MainEngine(Grid^, TextBox^);
		
		// Particles
		void SpawnParticle(ParticleProperties ^ properties);
		void ClearAllParticles();

		// Forces
		void SpawnGravitationalPoint(double xPos, double yPos, double mass);
		void SpawnLinearForce(double xMag, double yMag);
		void ClearAllForces();

		// Engine
		int Render(double);

	private:
		// Particles
		ParticleManager ^ particleManager;

		void SpawnParticle(double x, double y, double radius, double mass, Color color);
		void DeleteParticle(Particle ^);
		Particle^ SpawnParticle(double x, double y, double xVel, double yVel, double radius, double mass, Color color);

		// Forces
		GEN::List<Force ^> universalForces;

		void DeleteForce(Force ^);

		// Main loop
		void Update(double);
		void UpdateParticle(Particle ^);
		void HandleCollisions(GridOrganization ^ particleSet);
		void ParticleLoop(Particle ^);
		void FixParticlePosition(Particle ^);

		// UI
		Grid^ simGrid;
		GraphicsHandler ^ graphicsHandler;
		ConsoleHandler ^ console;
	};
}
