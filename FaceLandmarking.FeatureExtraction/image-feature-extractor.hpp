#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

#include "filters/basic-filter.hpp"
#include "hsv-image.hpp"
#include "hsv-image.hpp"

namespace FaceLandmarking::FeatureExtraction
{
	class ImageFeatureExtractor
	{
	private:
		HsvImage hsv;

	public:
		void setImage(const HsvImage& image)
		{
			hsv = image;
		}

		void selectFeatures(int x, int y, std::vector<float>& features)
		{
			for (auto channel : { HsvChannel::H, HsvChannel::S, HsvChannel::V })
			{
				const auto& layer = hsv[channel];

				for (const auto& filterSeed : Filters::BasicFilterSeeds)
				{
					Filters::BasicFiler filter(layer, filterSeed);

					float value = filter(x, y);

					features.push_back(value);
				}
			}
		}
	};
}