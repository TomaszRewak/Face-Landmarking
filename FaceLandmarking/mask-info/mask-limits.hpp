#pragma once

#include <vector>

namespace FaceLandmarking::MaskInfo
{
	struct Threshold : public std::vector<float>
	{ };

	struct MaskShapeLimits
	{
		std::vector<Threshold> angleThresholds;
		std::vector<Threshold> lengthThresholds;
	};

	struct MaskLimits
	{
		std::vector<MaskShapeLimits> shapeLimits;
	};
}