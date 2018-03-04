#include "stdafx.h"
#include "GraphicsHandler.h"


GraphicsHandler::GraphicsHandler(Grid^ simGrid) :
	simGrid(simGrid)
{
	
}

Shape2D^ GraphicsHandler::DrawCircle(double x, double y, double radius, Color color)
{
	Ellipse^ newCircle = gcnew Ellipse();
	newCircle->HorizontalAlignment = HorizontalAlignment::Left;
	newCircle->VerticalAlignment = VerticalAlignment::Bottom;

	newCircle->Height = radius * 2;
	newCircle->Width = radius * 2;
	newCircle->Fill = gcnew SolidColorBrush(color);

	Thickness circlePos = Thickness(0);
	circlePos.Bottom = y - radius;
	circlePos.Left = x - radius;

	newCircle->Margin = circlePos;

	simGrid->Children->Add(newCircle);

	return gcnew Shape2D(x, y, radius, newCircle);
}

void GraphicsHandler::RemoveShape(Shape2D^ shape)
{
	simGrid->Children->Remove(shape->shape);
}