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
#include "../FaceLandmarking.FaceLocator/face-finder.hpp"
#include "../FaceLandmarking.FaceLocator/mask-frame.hpp"
#include "ui/mask-ui.hpp"
#include "ui/face-ui.hpp"

using namespace cv;
using namespace std;
using namespace FaceLandmarking;

int camera_test()
{
	auto dataPath = experimental::filesystem::path("D:\\Programy\\FaceLandmarking\\Data");
	auto videoPath = "D:/Programy/FaceLandmarking/Data/examples/ja2.mp4";

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
		return -1;

	namedWindow("real", WINDOW_AUTOSIZE);

	Mat frame;
	Mat scaledFrame;
	Mat imageWithMasks;

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

		//{
		//	Mat frame2;
		//	resize(frame, frame2, Size(1200, 800));
		//	frame = frame2;
		//}

		frame.copyTo(imageWithMasks);

		for (auto& mask : masks)
		{
			float scale = maskFrame.getScale(mask);

			FaceMask normalizedMask = MaskTransformation::MaskTransition::scaleRelativeTo00(mask, scale);
			resize(frame, scaledFrame, cv::Size(frame.cols * scale, frame.rows * scale));

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
			return 0;
		}
	}

	return 0;
}
