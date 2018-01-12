#pragma once

#include <vector>
#include <limits>
#include <algorithm>

#include "math/point.hpp"
#include "math/size.hpp"
#include "math/rect.hpp"
#include "./mask-transformation/mask-offset.hpp"

namespace FaceLandmarking 
{
	class FaceMask : public std::vector<Math::Point<float>> 
	{
	public:
		Math::Point<float> faceCenter() const 
		{
			float x = 0;
			float y = 0;
			int points = size();

			for (const auto& point : *this)
			{
				x += point.x;
				y += point.y;
			}

			return Math::Point<float>(x / points, y / points);
		}

		Math::Size<float> faceSize() const
		{
			float
				minX = std::numeric_limits<float>::max(),
				maxX = std::numeric_limits<float>::min(),
				minY = std::numeric_limits<float>::max(),
				maxY = std::numeric_limits<float>::min();

			for (const auto& point : *this)
			{
				minX = std::min(minX, point.x);
				maxX = std::max(maxX, point.x);
				minY = std::min(minY, point.y);
				maxY = std::max(maxY, point.y);
			}

			return Math::Size<float>(maxX - minX, maxY - minY);
		}

		Math::Rect<float> faceRect() const
		{
			return Math::Rect<float>(
				faceCenter(),
				faceSize()
			);
		}
	};
}