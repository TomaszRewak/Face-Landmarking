#pragma once

#include <filesystem>
#include <iostream>

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

		std::array<IO::FeaturesIO<FeatureExtraction::FeatureExtractor::Length>, N> ios;

	public:
		RegressorProcessing(fs::path dataPath) :
			dataPath(dataPath),
			avgMask(IO::MaskIO<N>::load(dataPath / "mask" / "avg-face.mask"))
		{ }

		template<typename DatasetIterator>
		void compute(DatasetIterator begin, DatasetIterator end)
		{
			std::cout << std::endl << "Regressor processing";

			auto dir = dataPath / "features";

			fs::remove_all(dir);
			fs::create_directory(dir);

			for (int i = 0; i < ios.size(); i++)
				ios[i].open(dir, i);

			for (auto iter = begin; iter != end; ++iter)
			{
				auto example = *iter;

				if (colorTest.isBackAndWhite(example.image))
					continue;

				example.scaleFace(200, 200);

				FeatureExtraction::HsvImage processedImage;
				imagePreprocessor.processImage(example.image, processedImage, example.mask.faceRect() * 0.5, true);

				FeatureExtraction::FeatureExtractor featureSelector(processedImage);
				compute(featureSelector, example);

				std::cout << ".";
			}

			for (int i = 0; i < ios.size(); i++)
				ios[i].close();
		}

	private:
		void compute(FeatureExtraction::FeatureExtractor& featureSelector, Data::LearningExample<N>& example)
		{
			auto normalizedAvgMask = Mask::MaskTransformation::MaskNormalizer<N>(example.mask.faceRect())(avgMask);

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
				compute(featureSelector, example, Mask::MaskTransformation::MaskInterpolator<N>(factor, false, false)(example.mask, normalizedAvgMask));
				compute(featureSelector, example, Mask::MaskTransformation::MaskInterpolator<N>(factor, false, true)(example.mask, normalizedAvgMask));
				compute(featureSelector, example, Mask::MaskTransformation::MaskInterpolator<N>(factor, true, false)(example.mask, normalizedAvgMask));
				compute(featureSelector, example, Mask::MaskTransformation::MaskInterpolator<N>(factor, true, true)(example.mask, normalizedAvgMask));
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
				compute(featureSelector, example, Mask::MaskTransformation::MaskTransitioner<N>(Math::Vector<float>(offsetFactor, 0))(example.mask));
				compute(featureSelector, example, Mask::MaskTransformation::MaskTransitioner<N>(Math::Vector<float>(-offsetFactor, 0))(example.mask));
				compute(featureSelector, example, Mask::MaskTransformation::MaskTransitioner<N>(Math::Vector<float>(0, offsetFactor))(example.mask));
				compute(featureSelector, example, Mask::MaskTransformation::MaskTransitioner<N>(Math::Vector<float>(0, -offsetFactor))(example.mask));
			}
		}

		void compute(FeatureExtraction::FeatureExtractor& featureSelector, Data::LearningExample<N>& example, const Mask::FaceMask<N>& currentMask)
		{
			for (size_t i = 0; i < N; i++)
			{
				auto features = featureSelector(Math::Point<int>(currentMask[i].x, currentMask[i].y));
				auto decisions = FeatureExtraction::Decision<N>::getDecisions(example.mask, currentMask, i);

				ios[i].add(features, decisions);
			}
		}
	};
}