#pragma once

#include "../filters/basic-filter.hpp"

namespace FaceLandmarking::FeatureSelection::Test
{
	class FilterApplier
	{
	public:
		static cv::Mat applyFilter(const cv::Mat& baseImage, int filterIndex)
		{
			cv::Mat filteredImage;
			baseImage.copyTo(filteredImage);

			Filters::BasicFiler filter(baseImage, filterIndex);
			
			for (size_t c = 0; c < filteredImage.cols; c++) 
			{
				for (size_t r = 0; r < filteredImage.rows; r++)
				{
					float filterValue = filter(c, r);

					filteredImage.at<uchar>(r, c) = std::max(0., std::min(255., 125 + 50 * (filterValue + 0.5)));
				}
			}

			return filteredImage;
		}
	};
}