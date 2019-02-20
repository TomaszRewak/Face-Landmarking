#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <chrono>

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
#include "ui/video-capture.hpp"

namespace FaceLandmarking::App
{
	template<std::size_t N>
	void video(
		std::experimental::filesystem::path dataPath,
		UI::VideoCapture videoCapture
	)
	{
		const int FPS = 30;

		FaceLandmarking::FaceLandmarker<N> faceLandmarker(dataPath);

		cv::namedWindow("real", cv::WINDOW_AUTOSIZE);

		cv::Mat frame;
		cv::Mat frameWithMask;

		for (;;)
		{
			auto frameStart = std::chrono::system_clock::now();

			videoCapture.loadFrame(frame);

			frame.copyTo(frameWithMask);

			faceLandmarker.adjustMasks(frame, 15, 3);

			for (auto& mask : faceLandmarker.masks)
				UI::MaskUI<N>::drawMask(frameWithMask, mask);

			cv::imshow("real", frameWithMask);

			auto frameEnd = std::chrono::system_clock::now();

			auto key = cv::waitKey(1000 / FPS);
			if (key == 32) // space
				faceLandmarker.findFaces(frame);
			else if (key == 27) // escape
				return;
		}
	}
}