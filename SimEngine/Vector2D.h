#pragma once

#include <cmath>

struct Vector2D
{
public:
	Vector2D(double x, double y);
	Vector2D();

	static Vector2D NullVector() { return Vector2D(nan(""), nan("")); }

	double x;
	double y;
};

