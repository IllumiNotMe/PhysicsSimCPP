#include "stdafx.h"
#include "Shape2D.h"

using namespace System::Diagnostics;

SimEngine::Shape2D::Shape2D(double x, double y, double radius, Shape^ shape) :
	x(x), y(y), radius(radius), shape(shape)
{
}

void SimEngine::Shape2D::SetPosition(double xPos, double yPos)
{
	Thickness pos = Thickness(0);
	pos.Bottom = yPos;
	pos.Left = xPos;

	shape->Margin = pos;

	x = xPos;
	y = yPos;
}