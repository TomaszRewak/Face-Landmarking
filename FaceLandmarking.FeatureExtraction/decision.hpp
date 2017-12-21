#pragma once

#include "../FaceLandmarking/face-mask.hpp"

namespace FaceLandmarking::FeatureExtraction
{
	class Decision
	{
	public:
		static void getDecisions(const FaceMask& face, const FaceMask& mask, size_t point, std::vector<float>& decisions)
		{
			const auto& facePoint = face[point];
			const auto& maskPoint = mask[point];

			decisions.push_back(facePoint.x - maskPoint.x);
			decisions.push_back(facePoint.y - maskPoint.y);
		}
	};
}