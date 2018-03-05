#pragma once

#include "Particle.h"

using namespace System::Collections::Generic;
using namespace System;
using namespace SimEngine;

namespace GEN = System::Collections::Generic;

ref class GridOrganization
{
public:
	GridOrganization(GEN::List<Particle ^>^);
	void Organize();

	GEN::Dictionary<Tuple<int, int> ^, GEN::List<Particle ^> ^> sortedParticles;

private:
	GEN::List<Particle ^> rawParticles;

	void AddToNode(Particle^ p, Tuple<int, int>^ nodePos);
};