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

template<size_t N>
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
	using DataSetReader = Reader::DatasetReader<N>;

	Learning::AverageMaskProcessing<N, DataSetReader> averageMaskProcessing(dataPath);
	FaceMask<N> averageMask = averageMaskProcessing.load();

	FaceLocator::FaceFinder faceFinder(dataPath / "haar" / "haarcascade_frontalface_default.xml");

	FaceLocator::MaskFrame<N> maskFrame(averageMask, Math::Size<float>(150, 150));
	std::vector<FaceMask<N>> masks;

	FeatureExtraction::ImagePreprocessor imagePreprocessor;

	Learning::Regressors::MaskTreeRegressor<N> treeRegressor(dataPath / "regressors" / "trees");
	Learning::MaskRegression<N, FeatureExtraction::ImageFeatureExtractor, Learning::Regressors::MaskTreeRegressor<N>> maskRegression(treeRegressor);

	Learning::Regressors::NNRegressor<Learning::Regressors::ReluActivation> autoencoderRegressor(dataPath / "regressors" / "nn" / "autoencoder");
	MaskTransformation::MaskAutoencoder<N, Learning::Regressors::NNRegressor<Learning::Regressors::ReluActivation>> maskAutoencoder(autoencoderRegressor);

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
			FaceMask<N> normalizedMask = MaskTransformation::MaskTransition<N>::scale(mask, scale, scale, Math::Point<float>(0, 0));

			resize(frame, scaledFrame, cv::Size(frame.cols * scale, frame.rows * scale));

			auto faceRect = maskFrame.getFrame(mask);
			auto normalizedFaceRect = maskFrame.getFrame(normalizedMask);

			imagePreprocessor.processImage(scaledFrame, processedFrame, normalizedFaceRect * 0.5, true);
			maskRegression.setImage(processedFrame);

			for (int i = 0; i < N; i++)
			{
				mask[i] += maskRegression.computeOffset(normalizedMask[i], i, steps, regressionSize) / scale;
			}
			mask = maskAutoencoder.passThrough(maskAutoencoder.passThrough(mask));

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

			faceFinder.locate(frame);

			for (auto rect : faceFinder)
			{
				FaceMask<N> mask = MaskTransformation::MaskNormalizer<N>::normalizeMask(averageMask, rect);
				masks.push_back(mask);
			}

			break;
		}
		case 27: // escape
			return;
		}
	}
}
