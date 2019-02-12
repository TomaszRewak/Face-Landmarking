#pragma once

#include "../mask/face-mask.hpp"

namespace FaceLandmarking::FeatureExtraction
{
	template<size_t N>
	class Decision
	{
	public:
		static Math::Vector<float> getDecisions(const Mask::FaceMask<N>& face, const Mask::FaceMask<N>& mask, size_t point)
		{
			const auto& facePoint = face[point];
			const auto& maskPoint = mask[point];

			return Math::Vector<float>(
				facePoint.x - maskPoint.x,
				facePoint.y - maskPoint.y);
		}
	};
}