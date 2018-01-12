#pragma once

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "../FaceLandmarking.FeatureExtraction/hsv-image.hpp"

namespace FaceLandmarking::FeatureExtraction
{
	class FaceColorDetector : public cv::Mat
	{
	public:
		void detectFaceColor(FeatureExtraction::HsvImage& hsvImage)
		{
			this->create(hsvImage.rows(), hsvImage.columns(), CV_32F);

			for (size_t x = 0; x < cols; x++) 
			{
				for (size_t y = 0; y < rows; y++)
				{
					float& value = at<float>(y, x);
					int
						h = hsvImage[FeatureExtraction::HsvChannel::H].at<uchar>(y, x),
						s = hsvImage[FeatureExtraction::HsvChannel::S].at<uchar>(y, x),
						v = hsvImage[FeatureExtraction::HsvChannel::V].at<uchar>(y, x);

					value = 1;

					if (h < 137 || h > 150)
						value *= 0.5;

					if (h < 132 || h > 155)
						value *= 0.25;

					if (h < 119 || h > 162)
						value *= 0.25;

					if (h < 107 || h > 170)
						value *= 0;

					if (s > 230)
						value *= 0;
				}
			}
		}
	};
}