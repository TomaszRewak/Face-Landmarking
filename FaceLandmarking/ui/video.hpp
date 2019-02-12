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
	int transformHeight
)
{
	FaceLandmarking::FaceLandmarker<N> faceLandmarker(dataPath);

	VideoCapture videoCapture(videoPath);
	if (!videoCapture.isOpened())
		return;

	namedWindow("real", WINDOW_AUTOSIZE);

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

		faceLandmarker.adjustMasks(frame, steps);

		for (auto& mask : faceLandmarker.masks)
			Test::UI::MaskUI<N>::drawMask(frameWithMask, mask);

		imshow("real", frameWithMask);

		switch (waitKey(30)) // How to improve?
		{
		case 32: // space
		{
			faceLandmarker.findFaces(frame);
			break;
		}
		case 27: // escape
			return;
		}
	}
}
