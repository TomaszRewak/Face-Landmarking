#pragma once

#include <opencv2/core.hpp>

#include "../FaceLandmarking/face-mask.hpp"

namespace FaceLandmarking::FeatureSelection
{
	class MaskFeatureSelection
	{
	public:
		void selectFeatures(const FaceMask& mask, size_t maskPoint, std::vector<float>& features)
		{
			int relatedPoints[]
			{
				maskPoint - 1,
				maskPoint + 1
			};

			for (size_t relatedPoint : relatedPoints)
			{
				relatedPoint = cv::borderInterpolate(relatedPoint, mask.size(), cv::BorderTypes::BORDER_WRAP);

				features.push_back(mask[relatedPoint].x);
				features.push_back(mask[relatedPoint].y);
			}
		}
	};
}