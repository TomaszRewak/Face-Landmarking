#pragma once

#include "vector.hpp"

namespace FaceLandmarking::Math {
	template<typename T>
	struct Point {
		T x;
		T y;

		Point()
		{ }

		Point(T x, T y): 
			x(x),
			y(y)
		{ }

		Point<T> operator+(const Vector<T>& vector) const
		{
			return Point<T>(x + vector.x, y + vector.y);
		}

		void operator+=(const Vector<T>& vector)
		{
			x += vector.x;
			y += vector.y;
		}

		Point<T> operator-(const Vector<T>& vector) const
		{
			return Point<T>(x - vector.x, y - vector.y);
		}

		Vector<T> to(const Point<T>& point) const
		{
			return Vector<T>(point.x - x, point.y - y);
		}
	};
}