#pragma once

#include "image-feature-selector.hpp"
#include "mask-feature-selector.hpp"

namespace FaceLandmarking::FeatureSelection
{
	class FeatureSelector
	{
	private:
		ImageFeatureSelector imageFeatrues;
		MaskFeatureSelection maskFeatures;

	public:
		void setImage(const cv::Mat& image)
		{
			imageFeatrues.setImage(image);
		}

		void selectFeatures(const FaceMask& mask, int maskPoint, std::vector<float>& features)
		{
			auto[x, y] = mask[maskPoint];

			imageFeatrues.selectFeatures(x, y, features);
			maskFeatures.selectFeatures(mask, maskPoint, features);
		}
	};
}