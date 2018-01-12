#pragma once

#include "hsv-image.hpp"
#include "histogram.hpp"
#include "face-color-detector.hpp"
#include "hsv-image.hpp"

namespace FaceLandmarking::FeatureExtraction
{
	class ImagePreprocessor
	{
	public:
		cv::Mat bluredImage;
		FeatureExtraction::Histogram histogram;
		FeatureExtraction::FaceColorDetector colorDetector;
		
	public:
		void processImage(cv::Mat& inImage, HsvImage& outImage, Math::Rect<float>& faceRect, bool fixHSV = true)
		{
			//cv::GaussianBlur(inImage, bluredImage, cv::Size(5, 5), 2.5, 2.5);

			outImage.setImage(inImage);
			outImage.addOffset(FeatureExtraction::HsvChannel::H, 128);

			if (fixHSV)
			{
				colorDetector.detectFaceColor(outImage);

				histogram.setImage(outImage[FeatureExtraction::HsvChannel::H], faceRect, colorDetector);
				int maxH = histogram.median();
				outImage.addOffset(FeatureExtraction::HsvChannel::H, 141 - maxH);

				histogram.setImage(outImage[FeatureExtraction::HsvChannel::S], faceRect, colorDetector);
				int maxS = histogram.median();
				outImage.add(FeatureExtraction::HsvChannel::S, 150 - maxS);

				histogram.setImage(outImage[FeatureExtraction::HsvChannel::V], faceRect, colorDetector);
				int maxV = histogram.median();
				outImage.add(FeatureExtraction::HsvChannel::V, 150 - maxV);
			}
		}
	};
}