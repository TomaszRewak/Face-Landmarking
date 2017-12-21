#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace FaceLandmarking::Reader::Validation
{
	class ImageColorTest
	{
	private:
		cv::Mat color;
		cv::Mat hsv[3];

	public:
		bool isBackAndWhite(const cv::Mat& image)
		{
			cv::cvtColor(image, color, cv::COLOR_BGR2HSV_FULL);
			cv::split(color, hsv);

			return !(cv::countNonZero(hsv[0]) || cv::countNonZero(hsv[1]));
		}
	};
}