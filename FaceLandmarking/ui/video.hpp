#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include "../data/dataset.hpp"
#include "../preprocessing/face-finder.hpp"
#include "../preprocessing/mask-frame.hpp"
#include "../regression/mask-regressor.hpp"
#include "../regression/mask-autoencoder.hpp"
#include "../regression/regressors/nn-regressor.hpp"
#include "../regression/regressors/tree-mask-regressor.hpp"
#include "../feature-extraction/feature-extractor.hpp"
#include "../feature-extraction/image-preprocessor.hpp"

#include "../face-landmarker.hpp"

#include "ui/mask-ui.hpp"
#include "ui/face-ui.hpp"

template<size_t N>
void video_test(
	experimental::filesystem::path dataPath,
	std::string videoPath,
	int steps,
	bool transform,
	int transformRotate,
	int transformWidth,
	int transformHeight,
	int regressionSize,
	bool debug
)
{
	FaceLandmarking::FaceLandmarker faceLandmarker(dataPath);

	VideoCapture videoCapture(videoPath);
	if (!videoCapture.isOpened())
		return;

	namedWindow("real", WINDOW_AUTOSIZE);

	if (debug)
	{
		namedWindow("face", WINDOW_AUTOSIZE);
		namedWindow("color", WINDOW_AUTOSIZE);
	}

	Mat frame;
	Mat frameWithMask;
	Mat frameTransform;

	for (;;)
	{
		videoCapture >> frame;

		if (frame.empty()) {
			videoCapture = VideoCapture(videoPath);
			continue;
		}

		if (transform)
		{
			cv::rotate(frame, frameTransform, transformRotate);
			frameTransform.copyTo(frame);
			resize(frame, frameTransform, Size(transformWidth, transformHeight));
			frameTransform.copyTo(frame);
		}

		frame.copyTo(frameWithMask);

		for (auto& mask : masks)
		{
			Test::UI::MaskUI<N>::drawMask(frameWithMask, mask);

			if (true)
			{
				cv::Mat processedFrameRGB;
				processedFrame.getImage(processedFrameRGB);

				Test::UI::MaskUI<N>::drawMask(processedFrameRGB, normalizedMask);

				Test::UI::FaceUI::drawFace(frameWithMask, faceRect, cv::Scalar(255, 255, 255));
				Test::UI::FaceUI::drawFace(processedFrameRGB, normalizedFaceRect, cv::Scalar(255, 255, 255));

				imshow("face", processedFrameRGB);
				imshow("color", imagePreprocessor.colorDetector);
			}
		}

		imshow("real", frameWithMask);

		auto key = waitKey(30);
		switch (key)
		{
		case 32: // space
		{
			masks.clear();

			for (auto rect : faceFinder.locate(frame))
				masks.push_back(MaskTransformation::MaskNormalizer<N>::normalizeMask(averageMask, rect));

			break;
		}
		case 27: // escape
			return;
		}
	}
}
