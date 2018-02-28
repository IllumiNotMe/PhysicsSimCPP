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
		MainEngine(Grid^, TextBox^);

		
		void SpawnParticle(ParticleProperties ^ properties);

		int Render(double);

	private:
		void SpawnParticle(double x, double y, double radius, Color color);

		Grid^ simGrid;
		GraphicsHandler ^ graphicsHandler;
		ParticleManager ^ particleManager;
		ConsoleHandler ^ console;

		Particle^ SpawnParticle(double x, double y, double xVel, double yVel, double radius, Color color);

		void Update(double);
		void ParticleLoop(Particle^, double);
	};
}
