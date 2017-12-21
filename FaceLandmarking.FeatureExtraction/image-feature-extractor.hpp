#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

#include "filters\basic-filter.hpp"

namespace FaceLandmarking::FeatureExtraction
{
	class ImageFeatureExtractor
	{
	public: // private
		cv::Mat baseImage;
		cv::Mat blur;
		cv::Mat color;
		cv::Mat hsv[3];

		void fixHue()
		{
			auto& hue = hsv[0];

			for (size_t x = 0; x < hue.cols; x++) {
				for (size_t y = 0; y < hue.rows; y++)
				{
					auto& pixel = hue.at<uchar>(y, x);
					pixel = (pixel + 128) % 256;
				}
			}
		}

	public:
		void setImage(const cv::Mat& image)
		{
			image.copyTo(baseImage);

			cv::GaussianBlur(baseImage, blur, cv::Size(5, 5), 2.5, 2.5);
			cv::cvtColor(blur, color, cv::COLOR_BGR2HSV_FULL);
			cv::split(color, hsv);

			fixHue();
		}

		void selectFeatures(int x, int y, std::vector<float>& features) const
		{
			for (const auto& layer : hsv)
			{
				for (const auto& filterSeed : Filters::BasicFilterSeeds)
				{
					Filters::BasicFiler filter(layer, filterSeed);

					features.push_back(filter(x, y));
				}
			}
		}
	};
}