#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../../FaceLandmarking/face-mask.hpp"

namespace FaceLandmarking::Test::UI
{
	template<size_t N>
	class MaskUI
	{
	public:
		static void drawMask(const cv::Mat& image, const FaceMask<N>& mask, cv::Scalar color = cv::Scalar(255, 255, 0))
		{
			for (size_t pointIndex = 0; pointIndex < N; pointIndex++)
			{
				const auto& point = mask[pointIndex];

				cv::circle(
					image,
					cv::Point(point.x, point.y),
					2,
					color,
					1);

				//cv::putText(
				//	image,
				//	std::to_string(pointIndex),
				//	cv::Point(point.x, point.y),
				//	cv::FONT_HERSHEY_SIMPLEX,
				//	0.3,
				//	cv::Scalar(125, 125, 0));
			}
		}
	};
}