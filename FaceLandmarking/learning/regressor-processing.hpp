#pragma once

#include <filesystem>

#include "average-mask-processing.hpp"
#include "../learning/decision.hpp"
#include "../mask/mask-transformation/mask-interpolator.hpp"
#include "../mask/mask-transformation/mask-transitioner.hpp"
#include "../io/features-io.hpp"
#include "../feature-extraction/image-color-test.hpp"
#include "../feature-extraction/feature-extractor.hpp"
#include "../feature-extraction/image-preprocessor.hpp"

namespace FaceLandmarking::Learning
{
	namespace fs = std::experimental::filesystem;

	template<size_t N>
	class RegressorProcessing
	{
	private:
		fs::path dataPath;
		FeatureExtraction::ImagePreprocessor imagePreprocessor;
		FeatureExtraction::ImageColorTest colorTest;
		Mask::FaceMask<N> avgMask;

		std::array<IO::FeaturesIO<N>, N> ios;

	public:
		RegressorProcessing(fs::path dataPath) :
			avgMask(IO::MaskIO<N>::load(dataPath / "mask" / "avg-face.mask"))
		{ }

		template<typename DatasetIterator>
		void compute(DatasetIterator begin, DatasetIterator end)
		{
			auto dir = dataPath / "features";

			fs::remove_all(dir);
			fs::create_directory(dir);

			for (int i = 0; i < ios.size(); i++)
				ios[i].open(i, dir);

			DatasetReader reader(dataPath);

			for (auto iter = begin; iter != end; iter++)
			{
				auto example = *iter;

				if (colorTest.isBackAndWhite(example.image))
					continue;

				example.scaleFace(200, 200);

				FeatureExtraction::HsvImage processedImage;
				imagePreprocessor.processImage(example.image, processedImage, example.mask.faceRect() * 0.5, true);

				FeatureExtraction::FeatureExtractor featureSelector(processedImage);
				compute(example);
			}

			for (int i = 0; i < ios.size(); i++)
				ios[i].close();
		}

	private:
		void compute(FeatureExtraction::FeatureExtractor& featureSelector, Data::LearningExample<N>& example)
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

		void compute(FeatureExtraction::FeatureExtractor& featureSelector, Data::LearningExample<N>& example, const Mask::FaceMask<N>& currentMask)
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