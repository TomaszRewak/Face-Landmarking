#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include "../FaceLandmarking.Reader/dataset-reader.hpp"
#include "../FaceLandmarking.Reader/dataset-reducing-reader.hpp"
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

template<size_t N>
void example_test(experimental::filesystem::path dataPath, string mask)
{
	using DataSetReader = Reader::DatasetReader<N>;

	Learning::AverageMaskProcessing<N, DataSetReader> averageMaskProcessing(dataPath);
	FaceMask<N> averageMask = averageMaskProcessing.load();

	std::vector<float> features;
	std::vector<float> decisions;

	FeatureExtraction::ImagePreprocessor imagePreprocessor;

	Learning::Regressors::MaskTreeRegressor<N> treeRegressor(dataPath / "regressors" / "trees");
	Learning::MaskRegression<N, FeatureExtraction::ImageFeatureExtractor, Learning::Regressors::MaskTreeRegressor<N>> maskRegression(treeRegressor);

	Learning::Regressors::NNRegressor<Learning::Regressors::ReluActivation> autoencoderRegressor(dataPath / "regressors" / "nn" / "autoencoder");
	MaskTransformation::MaskAutoencoder<N, Learning::Regressors::NNRegressor<Learning::Regressors::ReluActivation>> maskAutoencoder(autoencoderRegressor);

	namedWindow("example", WINDOW_AUTOSIZE);

	Mat imageWithMasks;

	DataSetReader reader(dataPath);
	while (true)
	{
		if (reader.hasNext()) {
			auto example = reader.loadNext();
			example.scaleFace(200, 200); // Maybe smaller? 

			FeatureExtraction::HsvImage processedImage;
			imagePreprocessor.processImage(example.image, processedImage, example.mask.faceRect(), false);
			maskRegression.setImage(processedImage);

			auto normalizedMask = MaskTransformation::MaskNormalizer<N>::normalizeMask(example.mask, Math::Rect<float>(Math::Point<float>(50, 50), Math::Size<float>(100, 100)));
			auto averageScaledMask = MaskTransformation::MaskNormalizer<N>::normalizeMask(averageMask, Math::Rect<float>(example.mask.faceCenter(), example.mask.faceSize()));

			auto adjustedMask = averageScaledMask;

			while (true)
			{
				for (int i = 0; i < N; i++)
				{
					adjustedMask[i] += maskRegression.computeOffset(adjustedMask[i], i, 10, 2);
				}
				adjustedMask = maskAutoencoder.passThrough(adjustedMask);

				example.image.copyTo(imageWithMasks);
				Test::UI::MaskUI<N>::drawMask(imageWithMasks, example.mask);
				Test::UI::MaskUI<N>::drawMask(imageWithMasks, averageScaledMask, cv::Scalar(0, 0, 255));
				Test::UI::MaskUI<N>::drawMask(imageWithMasks, adjustedMask, cv::Scalar(255, 255, 255));
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
