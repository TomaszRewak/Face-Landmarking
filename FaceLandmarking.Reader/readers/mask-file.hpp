#pragma once

#include "../FaceLandmarking/face-mask.hpp"

namespace FaceLandmarking::Reader 
{
	template<size_t N>
	struct MaskFile
	{
		FaceMask<N> mask;
		std::string imageName;
	};
}