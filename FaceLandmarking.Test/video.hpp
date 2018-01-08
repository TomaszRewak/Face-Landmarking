#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include "../FaceLandmarking.Reader/dataset-reader.hpp"
#include "../FaceLandmarking.Reader/mask-io.hpp"
#include "../FaceLandmarking.Reader/validation/image-color-test.hpp"
#include "../FaceLandmarking.Reader/mask-description-io.hpp"
#include "../FaceLandmarking/mask-transformation/mask-normalizer.hpp"
#include "../FaceLandmarking/mask-transformation/mask-fixer.hpp"
#include "../FaceLandmarking.Learning/average-mask-processing.hpp"
#include "../FaceLandmarking.Learning/feature-processing.hpp"
#include "../FaceLandmarking.Learning/mask-regression.hpp"
#include "../FaceLandmarking.Learning/regressors/tree-regressor.hpp"
#include "../FaceLandmarking.FeatureExtraction/image-feature-extractor.hpp"
#include "../FaceLandmarking.FeatureExtraction/test/FilterApplier.hpp"
#include "../FaceLandmarking.FeatureExtraction/feature-extractor.hpp"
#include "../FaceLandmarking.FeatureExtraction/histogram.hpp"
#include "../FaceLandmarking.FeatureExtraction/hsv-image.hpp"
#include "../FaceLandmarking.FaceLocator/face-finder.hpp"
#include "../FaceLandmarking.FaceLocator/mask-frame.hpp"
#include "ui/mask-ui.hpp"
#include "ui/face-ui.hpp"

using namespace cv;
using namespace std;
using namespace FaceLandmarking;

void video_test(string videoPath)
{
	auto dataPath = experimental::filesystem::path("D:\\Programy\\FaceLandmarking\\Data");

	Reader::MaskDescriptionIO maskDescriptionIO(dataPath / "mask" / "mask-description.mask");
	MaskInfo::MaskDescription maskDescription = maskDescriptionIO.load();

	Learning::AverageMaskProcessing averageMaskProcessing(dataPath);
	FaceMask averageMask = averageMaskProcessing.load();

	Learning::MaskLimitsProcessing maskLimitsProcessing(maskDescription, dataPath);
	MaskInfo::MaskLimits maskLimits = maskLimitsProcessing.load();

	FaceLocator::FaceFinder faceFinder("D:\\Programy\\FaceLandmarking\\Data\\haar\\haarcascade_frontalface_default.xml");

	FaceLocator::MaskFrame maskFrame(averageMask, maskDescription, Math::Size<float>(200, 200));
	std::vector<FaceMask> masks;

	FeatureExtraction::ImageFeatureExtractor featureExtractor;
	Learning::Regressors::MaskTreeRegressor treeRegressor(dataPath / "regressors" / "trees");

	Learning::MaskRegression<FeatureExtraction::ImageFeatureExtractor, Learning::Regressors::MaskTreeRegressor> maskRegression(maskDescription, featureExtractor, treeRegressor);
	MaskTransformation::MaskFixer maskFixer(maskDescription, maskLimits);

	//VideoCapture videoCapture(0);
	VideoCapture videoCapture(videoPath);
	if (!videoCapture.isOpened())
		return;

	namedWindow("real", WINDOW_AUTOSIZE);
	namedWindow("face", WINDOW_AUTOSIZE);

	Mat frame;
	Mat scaledFrame;
	Mat imageWithMasks;
	Mat faceImage;

	for (;;)
	{
		videoCapture >> frame;

		if (frame.empty()) {
			videoCapture = VideoCapture(videoPath);
			continue;
		}

		{
			Mat frame2;
			cv::rotate(frame, frame2, cv::ROTATE_90_COUNTERCLOCKWISE);
			frame = frame2;
			resize(frame, frame2, Size(350, 600));
			frame = frame2;
		}

		frame.copyTo(imageWithMasks);

		for (auto& mask : masks)
		{
			float scale = maskFrame.getScale(mask);

			FaceMask normalizedMask = MaskTransformation::MaskTransition::scaleRelativeTo00(mask, scale);
			resize(frame, scaledFrame, cv::Size(frame.cols * scale, frame.rows * scale));

			auto faceRect = maskFrame.getFrame(mask);
			auto normalizedFaceRect = maskFrame.getFrame(normalizedMask);

			{
				FeatureExtraction::HsvImage hsvImage;
				FeatureExtraction::Histogram histogram;

				hsvImage.setImage(scaledFrame);
				hsvImage.addOffset(FeatureExtraction::HsvChannel::H, 128);

				histogram.setImage(hsvImage[FeatureExtraction::HsvChannel::S], normalizedFaceRect);
				int maxS = histogram.max(10, false);
				hsvImage.add(FeatureExtraction::HsvChannel::S, 150 - maxS);

				histogram.setImage(hsvImage[FeatureExtraction::HsvChannel::H], normalizedFaceRect);
				int maxH = histogram.max(10, false);
				hsvImage.addOffset(FeatureExtraction::HsvChannel::H, 140 - maxH);

				hsvImage.addOffset(FeatureExtraction::HsvChannel::H, 128);
				hsvImage.getImage(scaledFrame);
			}

			featureExtractor.setImage(scaledFrame);

			for (int i = 0; i < 15; i++)
			{
				maskRegression.compute(normalizedMask);
				maskRegression.apply(normalizedMask);
				maskRegression.apply(mask, 1 / scale);

				maskFixer.compute(normalizedMask);
				maskFixer.apply(normalizedMask);
				maskFixer.apply(mask, 1 / scale);
			}

			Test::UI::MaskUI::drawMask(imageWithMasks, mask, maskDescription);
			Test::UI::FaceUI::drawFace(imageWithMasks, faceRect, cv::Scalar(255, 255, 255));
			Test::UI::FaceUI::drawFace(scaledFrame, normalizedFaceRect, cv::Scalar(255, 255, 255));

			imshow("face", scaledFrame);
		}

		for (auto& face : faceFinder)
			Test::UI::FaceUI::drawFace(imageWithMasks, face);

		imshow("real", imageWithMasks);

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
