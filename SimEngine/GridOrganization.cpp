#include "stdafx.h"
#include "GridOrganization.h"


GridOrganization::GridOrganization(List<Particle ^>^ unsortedParticles) :
	rawParticles(unsortedParticles)
{
}

void GridOrganization::Organize()
{
	/*
	grid_scale = 18

	def organize(self):
	self.organized = {}
	for particle in self.particles:
	# Calculate grid position
	grid_x = particle.x / self.grid_scale
	grid_y = particle.y / self.grid_scale

	# Floored grid position (bottom left node)
	fgrid_x = math.floor(grid_x)
	fgrid_y = math.floor(grid_y)

	# Grid squares that the particle is contained in
	squares = [(fgrid_x, fgrid_y)]

	# This assumes particle width == grid_scale
	if grid_x > fgrid_x:
	squares.append((fgrid_x + 1, fgrid_y))
	if grid_y > fgrid_y:
	squares.append((fgrid_x, fgrid_y + 1))
	if len(squares) == 3:
	squares.append((fgrid_x + 1, fgrid_y + 1))

	for square in squares:
	if square in self.organized:
	self.organized[square].append(particle)
	else:
	self.organized[square] = [particle]
	*/
	double gridScale = 20.0;

	for each (Particle^ p in rawParticles)
	{
		// Calculate exact grid position
		double gridX = p->position().x / gridScale;
		double gridY = p->position().y / gridScale;

		// Grid node position
		int fGridX = int(gridX);
		int fGridY = int(gridY);

		// Nodes which the particle is a part of
		int totalNodes = 1;
		AddToNode(p, gcnew Tuple<int, int>(fGridX, fGridY));

		if (gridX > fGridX)
		{
			AddToNode(p, gcnew Tuple<int, int>(fGridX + 1, fGridY));
			totalNodes += 1;
		}
		if (gridY > fGridY)
		{
			AddToNode(p, gcnew Tuple<int, int>(fGridX, fGridY + 1));
			totalNodes += 1;
		}
		if (totalNodes == 3)
			AddToNode(p, gcnew Tuple<int, int>(fGridX + 1, fGridY + 1));

	}
}

void GridOrganization::AddToNode(Particle^ p, Tuple<int, int>^ nodePos)
{
	if (sortedParticles.ContainsKey(nodePos))
	{
		sortedParticles[nodePos]->Add(p);
	}
	else
	{
		GEN::List<Particle ^>^ newList = gcnew GEN::List<Particle ^>();
		newList->Add(p);
		sortedParticles[nodePos] = newList;
	}
}