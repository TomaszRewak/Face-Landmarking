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
			}

			/*for (size_t pointIndex = 0; pointIndex < N - 1; pointIndex++)
			{
				const auto& point1 = mask[pointIndex];
				const auto& point2 = mask[pointIndex + 1];

				cv::line(
					image,
					cv::Point(point1.x, point1.y),
					cv::Point(point2.x, point2.y),
					color,
					1);
			}*/
		}
	};
}