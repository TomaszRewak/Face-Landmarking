#pragma once

#include "point.hpp"
#include "size.hpp"

namespace FaceLandmarking::Math
{
	template<typename T>
	struct Rect
	{
		Point<T> center;
		Size<T> size;

		Rect()
		{ }

		Rect(float topLeftX, float topLeftY, float width, float height):
			center(topLeftX + width / 2, topLeftY + height / 2),
			size(width, height)
		{ }

		Rect(Point<T> center, Size<T> size) :
			center(center),
			size(size)
		{ }
	};
}