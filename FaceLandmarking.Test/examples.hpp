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
#include "../FaceLandmarking.FeatureExtraction/hsv-image.hpp"
#include "../FaceLandmarking.FeatureExtraction/image-preprocessing.hpp"
#include "ui/mask-ui.hpp"

using namespace cv;
using namespace std;
using namespace FaceLandmarking;

void example_test(experimental::filesystem::path dataPath, string mask)
{
	Learning::AverageMaskProcessing averageMaskProcessing(dataPath);
	FaceMask averageMask = averageMaskProcessing.load();

	std::vector<float> features;
	std::vector<float> decisions;

	FeatureExtraction::ImagePreprocessor imagePreprocessor;

	Learning::Regressors::MaskTreeRegressor treeRegressor(dataPath / "regressors" / "trees");
	Learning::MaskRegression<FeatureExtraction::ImageFeatureExtractor, Learning::Regressors::MaskTreeRegressor> maskRegression(treeRegressor);

	Learning::Regressors::NNRegressor<Learning::Regressors::LogisticActivation> autoencoderRegressor(dataPath / "regressors" / "nn" / "autoencoder");
	MaskTransformation::MaskAutoencoder<Learning::Regressors::NNRegressor<Learning::Regressors::LogisticActivation>> maskAutoencoder(autoencoderRegressor);

	namedWindow("example", WINDOW_AUTOSIZE);

	Mat imageWithMasks;

	Reader::DatasetReader reader(dataPath);
	while (true)
	{
		if (reader.hasNext()) {
			auto example = reader.loadNext();
			example.scaleFace(200, 200);

			FeatureExtraction::HsvImage processedImage;
			imagePreprocessor.processImage(example.image, processedImage, example.mask.faceRect(), false);
			maskRegression.setImage(processedImage);

			auto normalizedMask = MaskTransformation::MaskNormalizer::normalizeMask(example.mask, Math::Rect<float>(Math::Point<float>(50, 50), Math::Size<float>(100, 100)));
			auto averageScaledMask = MaskTransformation::MaskNormalizer::normalizeMask(averageMask, Math::Rect<float>(example.mask.faceCenter(), example.mask.faceSize()));

			auto adjustedMask = averageScaledMask;

			while (true)
			{
				for (int i = 0; i < 10; i++) {
					maskRegression.compute(adjustedMask);
					maskRegression.apply(adjustedMask);
				}

				adjustedMask = maskAutoencoder.passThrough(adjustedMask);

				example.image.copyTo(imageWithMasks);

				Test::UI::MaskUI::drawMask(imageWithMasks, example.mask);
				Test::UI::MaskUI::drawMask(imageWithMasks, averageScaledMask, cv::Scalar(0, 0, 255));
				Test::UI::MaskUI::drawMask(imageWithMasks, adjustedMask, cv::Scalar(255, 255, 255));
				imshow("example", imageWithMasks);

				auto key = waitKey(250000);
				if (key == 32)
					continue;
				else if (key == 27)
					return;
				else
					break;
			}
		}
	}
}
