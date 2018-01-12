#pragma once

#include <algorithm>

#include "point.hpp"

namespace FaceLandmarking::Math
{
	template<typename T>
	struct Angle
	{
		static T between(const Point<T>& p1, const Point<T>& p2, const Point<T>& p3)
		{
			float ang1 = std::atan2f(p1.y - p2.y, p1.x - p2.x);
			float ang2 = std::atan2f(p3.y - p2.y, p3.x - p2.x);

			if (ang2 < ang1)
				ang2 += 2 * 3.14159;

			return ang2 - ang1;
		}
	};
}