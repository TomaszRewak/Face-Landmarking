#pragma once

#include <opencv2/core.hpp>
#include <vector>

namespace FaceLandmarking::FeatureExtraction::Filters
{
	const std::vector<cv::Mat> BasicFilterSeeds{
		cv::Mat{
			+1
		},
		cv::Mat{
			+1, 0, -1,
			+2, 0, -2,
			+1, 0, -1
		},
		cv::Mat{
			+1, +2, +1,
			 0,  0,  0,
			-1, -2, -1
		},
		cv::Mat{
			+1, +1, +1, +1, +1,
			+1, +1, +1, +1, +1,
			+1, +1, +1, +1, +1,
			+1, +1, +1, +1, +1,
			+1, +1, +1, +1, +1,
		},
		cv::Mat{
			 0, +1,  0, -1,  0,
			+1, +2,  0, -2, -1,
			+2, +4,  0, -4, -2,
			+1, +2,  0, -2, -1,
			 0, +1,  0, -1,  0
		},
		cv::Mat{
			 0, +1, +2, +1,  0,
			+1, +2, +4, +2, +1,
			 0,  0,  0,  0,  0,
			-1, -2, -4, -2, -1,
			 0, -1, -2, -1,  0,
		},
		cv::Mat{
			+4, +2,  0, -2, +4,
			+2, +1,  0, -1, +2,
			+1,  0,  0,  0, +1,
			+2, +1,  0, -1, +2,
			+4, +2,  0, -2, +4,
		},
		cv::Mat{
			+4, +2, +1, +2, +4,
			+2, +1,  0, +1, +2,
			 0,  0,  0,  0,  0,
			-2, -1,  0, -1, -2,
			-4, -2, -1, -2, -4,
		},
	};

	class BasicFiler
	{
	private:
		const cv::Mat& filter;
		const cv::Mat& image;
		const int xOffset;
		const int yOffset;

	public:
		BasicFiler(const cv::Mat& image, const cv::Mat& filter) :
			image(image),
			filter(filter),
			xOffset(filter.cols / 2),
			yOffset(filter.rows / 2)
		{ }

		BasicFiler(const cv::Mat& image, int filter) :
			BasicFiler(image, BasicFilterSeeds[filter])
		{ }

		float operator()(int x, int y) const
		{
			float sum = 0;

			int filterCols = filter.cols,
				filterRows = filter.rows,
				imageCols = image.cols,
				imageRows = image.rows;

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
	};
}