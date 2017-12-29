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
#include "ui/mask-ui.hpp"

using namespace cv;
using namespace std;
using namespace FaceLandmarking;

int camera_test()
{
	auto dataPath = experimental::filesystem::path("D:\\Programy\\FaceLandmarking\\Data");

	Reader::MaskDescriptionIO maskDescriptionIO(dataPath / "mask" / "mask-description.mask");
	MaskInfo::MaskDescription maskDescription = maskDescriptionIO.load();

	Learning::AverageMaskProcessing averageMaskProcessing(dataPath);
	FaceMask averageMask = averageMaskProcessing.load();

	Learning::MaskLimitsProcessing maskLimitsProcessing(maskDescription, dataPath);
	MaskInfo::MaskLimits maskLimits = maskLimitsProcessing.load();

	//averageMask = MaskTransformation::MaskNormalizer::normalizeMask(averageMask, Math::Point<float>(300, 300), Math::Size<float>(200, 200));
	averageMask = MaskTransformation::MaskNormalizer::normalizeMask(averageMask, Math::Point<float>(120, 350), Math::Size<float>(200, 200));

	auto faceCascade = cv::CascadeClassifier("D:\\Programy\\FaceLandmarking\\Data\\haar\\haarcascade_frontalface_default.xml");
	auto eyeCascade = cv::CascadeClassifier("D:\\Programy\\FaceLandmarking\\Data\\haar\\haarcascade_eye.xml");
	auto mouthCascade = cv::CascadeClassifier("D:\\Programy\\FaceLandmarking\\Data\\haar\\haarcascade_smile.xml");

	auto mask = averageMask;

	FeatureExtraction::ImageFeatureExtractor featureExtractor;
	Learning::Regressors::MaskTreeRegressor treeRegressor(dataPath / "regressors" / "trees");

	Learning::MaskRegression<FeatureExtraction::ImageFeatureExtractor, Learning::Regressors::MaskTreeRegressor> maskRegression(maskDescription, featureExtractor, treeRegressor);
	MaskTransformation::MaskFixer maskFixer(maskDescription, maskLimits);

	//VideoCapture videoCapture(0);
	VideoCapture videoCapture("D:/Programy/FaceLandmarking/Data/examples/ja3.mp4");
	if (!videoCapture.isOpened())
		return -1;

	namedWindow("real", WINDOW_AUTOSIZE);

	Mat frame;
	Mat gray;
	Mat imageWithMasks;

	for (;;)
	{
		videoCapture >> frame;

		if (frame.empty()) {
			videoCapture = VideoCapture("D:/Programy/FaceLandmarking/Data/examples/ja3.mp4");
			continue;
		}

		Mat frame2;
		cv::rotate(frame, frame2, cv::ROTATE_90_COUNTERCLOCKWISE);
		frame = frame2;
		resize(frame, frame2, Size(350, 600));
		frame = frame2;

		featureExtractor.setImage(frame);

		for (int i = 0; i < 20; i++)
		{
			maskRegression.adjust(mask);
			maskFixer.fix(mask);
		}

		frame.copyTo(imageWithMasks);
		Test::UI::MaskUI::drawMask(imageWithMasks, mask, maskDescription);

		////

		//cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		//
		//std::vector<cv::Rect> faces;
		//faceCascade.detectMultiScale(gray, faces, 1.3, 5);
		//for (auto& face : faces)
		//	cv::rectangle(imageWithMasks, face, cv::Scalar(255, 0, 0), 2);
		//
		//std::vector<cv::Rect> eyes;
		//eyeCascade.detectMultiScale(gray, eyes, 1.3, 5);
		//for (auto& eye : eyes)
		//	cv::rectangle(imageWithMasks, eye, cv::Scalar(0, 255, 0), 2);
		//
		//std::vector<cv::Rect> mouths;
		//mouthCascade.detectMultiScale(gray, mouths, 1.3, 15);
		//for (auto& mouth : mouths)
		//	cv::rectangle(imageWithMasks, mouth, cv::Scalar(0, 0, 255), 2);

		////

		imshow("real", imageWithMasks);

		auto key = waitKey(30);
		if (key == 32)
			mask = averageMask;
		else if (key == 27)
			return 0;
	}

	return 0;
}
