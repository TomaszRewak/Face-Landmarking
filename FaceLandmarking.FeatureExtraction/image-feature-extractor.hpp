#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

#include "filters\basic-filter.hpp"
#include "hsv-image.hpp"

namespace FaceLandmarking::FeatureExtraction
{
	class ImageFeatureExtractor
	{
	private:
		cv::Mat blur;
		HsvImage hsv;

	public:
		void setImage(const cv::Mat& image)
		{
			cv::GaussianBlur(image, blur, cv::Size(5, 5), 2.5, 2.5);
			hsv.setImage(blur);

			hsv.addOffset(HsvChannel::H, 128);
		}

		void selectFeatures(int x, int y, std::vector<float>& features)
		{
			for (auto channel : { HsvChannel::H, HsvChannel::S, HsvChannel::V })
			{
				const auto& layer = hsv[channel];

				for (const auto& filterSeed : Filters::BasicFilterSeeds)
				{
					Filters::BasicFiler filter(layer, filterSeed);

					features.push_back(filter(x, y));
				}
			}
		}
	};
}