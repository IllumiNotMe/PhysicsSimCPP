#pragma once

using namespace System::Windows::Shapes;
using namespace System::Windows;

namespace SimEngine
{
	public ref class Shape2D
	{
	public:
		Shape2D(double x, double y, double radius, Shape^);
		Shape^ shape;
		void SetPosition(double x, double y);

	private:
		double x;
		double y;
		double radius;
	};
}