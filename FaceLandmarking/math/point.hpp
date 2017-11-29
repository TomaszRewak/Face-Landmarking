#pragma once

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
	};
}