#pragma once

#include "basic-filters.hpp"

namespace FaceLandmarking::FeatureExtraction
{
	class Filter
	{
	public:
		static const std::size_t Length = BasicFilters.size();

		Filter(const cv::Mat& image, int filter) :
			image(image),
			filter(BasicFilters[filter])
		{ }

		float operator()(int x, int y) const
		{
			float sum = 0;

			int filterCols = filter.cols,
				filterRows = filter.rows,
				imageCols = image.cols,
				imageRows = image.rows,
				xOffset = filter.rows / 2,
				yOffset = filter.cols / 2;

			for (size_t c = 0; c < filterCols; c++)
			{
				for (size_t r = 0; r < filterRows; r++)
				{
					int filterValue = filter.at<int>(r, c);
					int imageValue = image.at<uchar>(
						cv::borderInterpolate(y - yOffset + r, imageRows, cv::BorderTypes::BORDER_REPLICATE),
						cv::borderInterpolate(x - xOffset + c, imageCols, cv::BorderTypes::BORDER_REPLICATE)
						);

					sum += filterValue * imageValue / 255.;
				}
			}

			return sum;
		}

	private:
		const cv::Mat& filter;
		const cv::Mat& image;
	};
}