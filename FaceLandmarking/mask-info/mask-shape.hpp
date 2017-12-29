#pragma once

#include <string>

namespace FaceLandmarking::MaskInfo {
	struct MaskShape
	{
		std::vector<size_t> points;

		bool closed = false;
		bool visible = true;

		std::string name;

		size_t point(int pointIndex) const
		{
			int size = points.size();

			while (pointIndex < 0)
				pointIndex += size;
			while (pointIndex >= size)
				pointIndex -= size;

			return points[pointIndex];
		}

		size_t firstEnclosed() const 
		{
			return closed ? 0 : 1;
		}

		size_t lastEnclosed() const
		{
			return points.size() - (closed ? 1 : 2);
		}
	};
}