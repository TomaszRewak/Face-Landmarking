#pragma once

#include <opencv2/core.hpp>

#include "../FaceLandmarking/face-mask.hpp"

namespace FaceLandmarking::FeatureExtraction
{
	class MaskFeatureExtractor
	{
	public:
		void selectFeatures(const FaceMask& mask, size_t maskPoint, std::vector<float>& features)
		{
			int relatedPoints[]
			{
				//maskPoint - 4,
				maskPoint - 1,
				maskPoint + 1,
				//maskPoint + 4
			};

			for (size_t relatedPoint : relatedPoints)
			{
				relatedPoint = cv::borderInterpolate(relatedPoint, mask.size(), cv::BorderTypes::BORDER_WRAP);

				features.push_back(mask[relatedPoint].x - mask[maskPoint].x);
				features.push_back(mask[relatedPoint].y - mask[maskPoint].y);
			}
		}
	};
}