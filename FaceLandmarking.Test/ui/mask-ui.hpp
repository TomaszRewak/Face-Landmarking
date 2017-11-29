#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../../FaceLandmarking/face-mask.hpp"

namespace FaceLandmarking::Test::UI
{
	class MaskUI
	{
	public:
		static cv::Mat drawMask(const cv::Mat& image, const FaceMask& mask, cv::Scalar color = cv::Scalar(255, 255, 0))
		{
			cv::Mat example(image);

			for (const auto& point : mask)
			{
				cv::circle(
					example,
					cv::Point(point.x, point.y),
					2,
					color,
					1);
			}

			return example;
		}
	};
}