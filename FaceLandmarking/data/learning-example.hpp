#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "../mask/face-mask.hpp"

namespace FaceLandmarking::Data
{
	template<size_t N>
	struct LearningExample
	{
		cv::Mat image;
		Mask::FaceMask<N> mask;

		LearningExample(const cv::Mat& image, const Mask::FaceMask<N>& mask) :
			image(image),
			mask(mask)
		{ }

		void scaleFace(float maxWidth, float maxHeight)
		{
			auto[faceWidth, faceHeight] = mask.faceSize();

			float factor = std::min(
				maxWidth / faceWidth,
				maxHeight / faceHeight
			);

			cv::resize(image, image, cv::Size(image.cols * factor, image.rows * factor));
			
			for (auto& point : mask)
			{
				point.x *= factor;
				point.y *= factor;
			}
		}
	};
}