#pragma once

#include "../FaceLandmarking/face-mask.hpp"

namespace FaceLandmarking::FeatureExtraction
{
	template<size_t N>
	class Decision
	{
	public:
		static void getDecisions(const FaceMask<N>& face, const FaceMask<N>& mask, size_t point, std::vector<float>& decisions)
		{
			const auto& facePoint = face[point];
			const auto& maskPoint = mask[point];

			decisions.push_back(facePoint.x - maskPoint.x);
			decisions.push_back(facePoint.y - maskPoint.y);
		}
	};
}