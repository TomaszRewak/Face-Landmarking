#pragma once

#include <filesystem>

#include "average-mask-processing.hpp"
#include "../FaceLandmarking.FeatureExtraction/decision.hpp"
#include "../FaceLandmarking/mask-transformation/mask-interpolation.hpp"
#include "../FaceLandmarking/mask-transformation/mask-transition.hpp"
#include "../FaceLandmarking.Reader/features-io.hpp"
#include "../FaceLandmarking.Reader/validation/image-color-test.hpp"
#include "../FaceLandmarking.FeatureExtraction/feature-extractor.hpp"
#include "../FaceLandmarking.FeatureExtraction/image-preprocessing.hpp"

namespace FaceLandmarking::Learning
{
	namespace fs = std::experimental::filesystem;

	template<size_t N, typename DatasetReader>
	class FeatureProcessing
	{
	private:
		fs::path dataPath;
		FeatureExtraction::ImagePreprocessor imagePreprocessor;
		FeatureExtraction::FeatureExtractor<N> featureSelector;
		Reader::Validation::ImageColorTest colorTest;
		FaceMask<N> avgMask;

		std::array<Reader::FeaturesIO, N> ios;

	public:
		FeatureProcessing(fs::path dataPath) :
			dataPath(dataPath),
			avgMask(AverageMaskProcessing<N, DatasetReader>(dataPath).load())
		{ }

		void compute()
		{
			auto dir = dataPath / "features";

			fs::remove_all(dir);
			fs::create_directory(dir);

			for (int i = 0; i < ios.size(); i++)
				ios[i].open(i, dir);

			DatasetReader reader(dataPath);

			while (reader.hasNext())
			{
				auto example = reader.loadNext(true);

				if (colorTest.isBackAndWhite(example.image))
					continue;

				example.scaleFace(200, 200);

				FeatureExtraction::HsvImage processedImage;
				imagePreprocessor.processImage(example.image, processedImage, example.mask.faceRect(), false);
				featureSelector.setImage(processedImage);

				compute(example);
			}

			for (int i = 0; i < ios.size(); i++)
				ios[i].close();
		}

	private:
		void compute(Reader::LearningExample<N>& example)
		{
			auto normalizedAvgMask = MaskTransformation::MaskNormalizer<N>::normalizeMask(
				avgMask,
				example.mask.faceRect()
			);

			float interpolationFactors[]{
				0.,
				0.1,
				0.2,
				0.3,
				0.5,
				0.75,
				1.,
			};

			for (auto factor : interpolationFactors)
			{
				compute(example, MaskTransformation::MaskInterpolation<N>::interpolate(example.mask, normalizedAvgMask, factor, false, false));
				compute(example, MaskTransformation::MaskInterpolation<N>::interpolate(example.mask, normalizedAvgMask, factor, false, true));
				compute(example, MaskTransformation::MaskInterpolation<N>::interpolate(example.mask, normalizedAvgMask, factor, true, false));
				compute(example, MaskTransformation::MaskInterpolation<N>::interpolate(example.mask, normalizedAvgMask, factor, true, true));
			}

			float offsetFactors[]{
				2,
				5,
				10,
				25,
				50
			};

			for (auto offsetFactor : offsetFactors)
			{
				compute(example, MaskTransformation::MaskTransition<N>::moveMask(example.mask, Math::Vector<float>(offsetFactor, 0)));
				compute(example, MaskTransformation::MaskTransition<N>::moveMask(example.mask, Math::Vector<float>(-offsetFactor, 0)));
				compute(example, MaskTransformation::MaskTransition<N>::moveMask(example.mask, Math::Vector<float>(0, offsetFactor)));
				compute(example, MaskTransformation::MaskTransition<N>::moveMask(example.mask, Math::Vector<float>(0, -offsetFactor)));
			}
		}

		void compute(Reader::LearningExample<N>& example, const FaceMask<N>& currentMask)
		{
			for (size_t i = 0; i < N; i++)
			{
				std::vector<float> features;
				std::vector<float> decisions;

				featureSelector.selectFeatures(currentMask, i, features);
				FeatureExtraction::Decision<N>::getDecisions(example.mask, currentMask, i, decisions);

				ios[i].add(features, decisions);
			}
		}
	};
}