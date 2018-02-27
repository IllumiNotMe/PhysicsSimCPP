// SimEngine.h

#pragma once

#include <chrono>
#include <ctime>
#include "GraphicsHandler.h"
#include "ParticleManager.h"
#include "Shape2D.h"
#include "MathUtils.h"

using namespace System::Windows::Controls;
using namespace System::Windows::Shapes;
using namespace System::Windows::Media;
using namespace System::Windows;

using namespace std::chrono;

using namespace MathUtils;

namespace SimEngine {

	public ref class MainEngine
	{
	public:
		MainEngine(Grid^);

		void SpawnParticle(double x, double y, double radius, Color color);
		int Render(double);

	private:
		GraphicsHandler^ graphicsHandler;
		ParticleManager^ particleManager;

		void Update(double);
		static void ParticleLoop(Particle^, double);
	};
}
