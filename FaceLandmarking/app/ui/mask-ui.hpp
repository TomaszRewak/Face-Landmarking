#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../../mask/face-mask.hpp"

namespace FaceLandmarking::App::UI
{
	template<size_t N>
	class MaskUI
	{
	public:
		static void drawMask(const cv::Mat& image, const Mask::FaceMask<N>& mask)
		{
			for (size_t pointIndex = 0; pointIndex < N; pointIndex++)
			{
				const auto& point = mask[pointIndex];

				cv::circle(
					image,
					cv::Point(point.x, point.y),
					2,
					cv::Scalar(255, 255 * pointIndex / N, 255 - 255 * pointIndex / N),
					1);
			}
		}
	};
}