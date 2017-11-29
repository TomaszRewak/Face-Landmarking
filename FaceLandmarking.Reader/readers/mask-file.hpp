#pragma once

#include "../FaceLandmarking/face-mask.hpp"

namespace FaceLandmarking::Reader 
{
	struct MaskFile
	{
		FaceMask mask;
		std::string image;
	};
}