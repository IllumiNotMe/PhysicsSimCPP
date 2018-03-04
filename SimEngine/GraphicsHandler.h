#pragma once

#include "Shape2D.h"

using namespace System::Windows::Controls;
using namespace System::Windows::Shapes;
using namespace System::Windows::Media;
using namespace System::Windows;

private ref class GraphicsHandler
{
public:
	GraphicsHandler(Grid^);
	Shape2D^ DrawCircle(double x, double y, double radius, Color);
	void RemoveShape(Shape2D ^);
private:
	Grid^ simGrid;
};
