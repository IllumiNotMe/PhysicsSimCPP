#pragma once

using namespace System::Windows::Shapes;
using namespace System::Windows;

ref class Shape2D
{
public:
	Shape2D(double x, double y, double radius, Shape^);
	void SetPosition(double x, double y);

private:
	double x;
	double y;
	double radius;
	Shape^ shape;
};

