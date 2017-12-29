#pragma once

#include <vector>

#include "mask-shape.hpp"
#include "mask-point.hpp"

namespace FaceLandmarking::MaskInfo
{
	struct MaskDescription 
	{
		std::vector<MaskPoint> points;
		std::vector<MaskShape> shapes;
	};
}