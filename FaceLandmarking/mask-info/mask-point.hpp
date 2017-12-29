#pragma once

#include "../math/vector.hpp"

namespace FaceLandmarking::MaskInfo {
	struct MaskPoint
	{
		bool inUse = false;
		bool floating = false;

		Math::Vector<float> defaultDirection;
	};
}