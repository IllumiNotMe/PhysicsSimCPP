#include "stdafx.h"
#include "Shape2D.h"


Shape2D::Shape2D(double x, double y, double radius, Shape^ shape) :
	x(x), y(y), radius(radius), shape(shape)
{
}

void Shape2D::SetPosition(double xPos, double yPos)
{
	x = xPos;
	y = yPos;

	Thickness pos = Thickness(0);
	pos.Bottom = y;
	pos.Left = x;

	shape->Margin = pos;
}