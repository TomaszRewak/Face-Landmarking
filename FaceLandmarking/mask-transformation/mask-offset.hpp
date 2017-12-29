#pragma once

#include <vector>
#include "../math/vector.hpp"

namespace FaceLandmarking::MaskTransformation
{
	class MaskOffset : public std::vector<Math::Vector<float>>
	{
	};
}

