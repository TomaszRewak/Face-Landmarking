#pragma once

#include "../FaceLandmarking/face-mask.hpp"

namespace FaceLandmarking::Data
{
	template<size_t N>
	struct MaskFile
	{
		FaceMask<N> mask;
		std::string imageName;
	};
}