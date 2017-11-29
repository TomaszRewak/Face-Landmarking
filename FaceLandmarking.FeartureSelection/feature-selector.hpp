#pragma once

#include <opencv2/core.hpp>

namespace FaceLandmarking::FeatureSelection
{
	class FeatureSelector
	{
	public:
		virtual void setImage(const cv::Mat& image) = 0;
		virtual void selectFeatures(cv::Mat *features, int x, int y) const = 0;
	};
}