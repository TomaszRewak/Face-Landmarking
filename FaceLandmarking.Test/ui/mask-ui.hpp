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

			for (size_t pointIndex = 0; pointIndex < mask.size(); pointIndex++)
			{
				const auto& point = mask[pointIndex];
				bool isMouthPoint = pointIndex >= 58 && pointIndex <= 113;

				cv::Scalar color_mouth = cv::Scalar(255, 0, 0);

				cv::circle(
					example,
					cv::Point(point.x, point.y),
					2,
					isMouthPoint ? color : color / 2,
					1);

				//cv::putText(
				//	example,
				//	std::to_string(pointIndex),
				//	cv::Point(point.x, point.y),
				//	cv::FONT_HERSHEY_SIMPLEX,
				//	0.3,
				//	cv::Scalar(125, 125, 0));
			}

			return example;
		}
	};
}