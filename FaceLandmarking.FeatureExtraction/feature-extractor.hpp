#pragma once

#include "image-feature-extractor.hpp"
#include "mask-feature-extractor.hpp"

namespace FaceLandmarking::FeatureExtraction
{
	class FeatureExtractor
	{
	private:
		ImageFeatureExtractor imageFeatrues;
		MaskFeatureExtractor maskFeatures;

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