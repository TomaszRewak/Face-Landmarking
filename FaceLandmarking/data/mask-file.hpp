#pragma once

#include "../mask/face-mask.hpp"

namespace FaceLandmarking::Data
{
	template<size_t N>
	struct MaskFile
	{
		Mask::FaceMask<N> mask;
		std::string imageName;
	};
}