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

namespace FaceLandmarking::App
{
	template<std::size_t N>
	void video(
		std::experimental::filesystem::path dataPath,
		std::string videoPath,
		bool transform,
		int transformRotate,
		int transformWidth,
		int transformHeight
	)
	{
		FaceLandmarking::FaceLandmarker<N> faceLandmarker(dataPath);

		cv::VideoCapture videoCapture(videoPath);
		if (!videoCapture.isOpened())
			return;

		cv::namedWindow("real", cv::WINDOW_AUTOSIZE);

		cv::Mat frame;
		cv::Mat frameWithMask;
		cv::Mat frameTransform;

		for (;;)
		{
			videoCapture >> frame;

			if (frame.empty()) {
				videoCapture = cv::VideoCapture(videoPath);
				continue;
			}

			if (transform)
			{
				cv::rotate(frame, frameTransform, transformRotate);
				frameTransform.copyTo(frame);
				cv::resize(frame, frameTransform, cv::Size(transformWidth, transformHeight));
				frameTransform.copyTo(frame);
			}

			frame.copyTo(frameWithMask);

			faceLandmarker.adjustMasks(frame, 15, 3);

			for (auto& mask : faceLandmarker.masks)
				UI::MaskUI<N>::drawMask(frameWithMask, mask);

			cv::imshow("real", frameWithMask);

			auto key = cv::waitKey(30);
			if (key == 32) // space
				faceLandmarker.findFaces(frame);
			else if (key == 27) // escape
				return;
		}
	}
}