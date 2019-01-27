#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include "../FaceLandmarking.Reader/dataset-reader.hpp"
#include "../FaceLandmarking.Reader/mask-io.hpp"
#include "../FaceLandmarking.Reader/validation/image-color-test.hpp"
#include "../FaceLandmarking/mask-transformation/mask-normalizer.hpp"
#include "../FaceLandmarking/mask-transformation/mask-autoencoder.hpp"
#include "../FaceLandmarking.Learning/average-mask-processing.hpp"
#include "../FaceLandmarking.Learning/feature-processing.hpp"
#include "../FaceLandmarking.Learning/mask-regression.hpp"
#include "../FaceLandmarking.Learning/regressors/tree-regressor.hpp"
#include "../FaceLandmarking.Learning/regressors/nn-regressor.hpp"
#include "../FaceLandmarking.FeatureExtraction/image-feature-extractor.hpp"
#include "../FaceLandmarking.FeatureExtraction/test/FilterApplier.hpp"
#include "../FaceLandmarking.FeatureExtraction/feature-extractor.hpp"
#include "../FaceLandmarking.FeatureExtraction/histogram.hpp"
#include "../FaceLandmarking.FeatureExtraction/hsv-image.hpp"
#include "../FaceLandmarking.FeatureExtraction/image-preprocessing.hpp"
#include "../FaceLandmarking.FaceLocator/face-finder.hpp"
#include "../FaceLandmarking.FaceLocator/mask-frame.hpp"
#include "ui/mask-ui.hpp"
#include "ui/face-ui.hpp"

using namespace cv;
using namespace std;
using namespace FaceLandmarking;

void video_test(
	experimental::filesystem::path dataPath,
	string videoPath,
	string mask,
	int steps,
	bool transform,
	int transformRotate,
	int transformWidth,
	int transformHeight,
	int regressionSize,
	bool debug
)
{
	Learning::AverageMaskProcessing averageMaskProcessing(dataPath);
	FaceMask averageMask = averageMaskProcessing.load();

	FaceLocator::FaceFinder faceFinder(dataPath / "haar" / "haarcascade_frontalface_default.xml");

	FaceLocator::MaskFrame maskFrame(averageMask, Math::Size<float>(200, 200));
	std::vector<FaceMask> masks;

	FeatureExtraction::ImagePreprocessor imagePreprocessor;

	Learning::Regressors::MaskTreeRegressor treeRegressor(dataPath / "regressors" / "trees");
	Learning::MaskRegression<FeatureExtraction::ImageFeatureExtractor, Learning::Regressors::MaskTreeRegressor> maskRegression(treeRegressor);

	Learning::Regressors::NNRegressor<Learning::Regressors::LogisticActivation> autoencoderRegressor(dataPath / "regressors" / "nn" / "autoencoder");
	MaskTransformation::MaskAutoencoder<Learning::Regressors::NNRegressor<Learning::Regressors::LogisticActivation>> maskAutoencoder(autoencoderRegressor);

	//VideoCapture videoCapture(0);
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
	Mat scaledFrame;
	Mat frameWithMask;
	Mat faceImage;
	Mat frameTransform;
	FeatureExtraction::HsvImage processedFrame;

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
			float scale = maskFrame.getScale(mask);

			FaceMask normalizedMask = MaskTransformation::MaskTransition::scale(mask, scale, scale, Math::Point<float>(0, 0));
			resize(frame, scaledFrame, cv::Size(frame.cols * scale, frame.rows * scale));

			auto faceRect = maskFrame.getFrame(mask);
			auto normalizedFaceRect = maskFrame.getFrame(normalizedMask);

			imagePreprocessor.processImage(scaledFrame, processedFrame, normalizedFaceRect * 0.7, true);
			maskRegression.setImage(processedFrame);

			for (int i = 0; i < steps; i++)
			{
				maskRegression.compute(normalizedMask, regressionSize);
				maskRegression.apply(normalizedMask);
				maskRegression.apply(mask, 1 / scale);
			}
			mask = maskAutoencoder.passThrough(mask);

			Test::UI::MaskUI::drawMask(frameWithMask, mask);

			if (debug)
			{
				cv::Mat processedFrameRGB;
				processedFrame.getImage(processedFrameRGB);

				Test::UI::MaskUI::drawMask(processedFrameRGB, normalizedMask);

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

			faceFinder.locate(frame);

			for (auto rect : faceFinder)
			{
				FaceMask mask = MaskTransformation::MaskNormalizer::normalizeMask(averageMask, rect);
				masks.push_back(mask);
			}

			break;
		}
		case 27: // escape
			return;
		}
	}
}
