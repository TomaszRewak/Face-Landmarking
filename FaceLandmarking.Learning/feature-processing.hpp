#pragma once

#include <filesystem>

#include "average-face.hpp"
#include "../FaceLandmarking.FeatureExtraction/decision.hpp"
#include "../FaceLandmarking/mask-transformation/mask-interpolation.hpp"
#include "../FaceLandmarking.Reader/features-io.hpp"
#include "../FaceLandmarking.Reader/validation/image-color-test.hpp"
#include "../FaceLandmarking.FeatureExtraction/feature-extractor.hpp"

namespace FaceLandmarking::Learning
{
	namespace fs = std::experimental::filesystem;

	class FeatureProcessing
	{
	private:
		fs::path dataPath;
		FeatureExtraction::FeatureExtractor featureSelector;
		Reader::Validation::ImageColorTest colorTest;
		Reader::FeaturesIO featuresIO;
		FaceMask avgMask;

	public:
		FeatureProcessing(fs::path dataPath) :
			dataPath(dataPath),
			featuresIO(dataPath / "features"),
			avgMask(AverageFace(dataPath).load())
		{ }

		void compute()
		{
			Reader::DatasetReader reader(dataPath);

			while (reader.hasNext())
			{
				auto example = reader.loadNext(true);
				example.scaleFace(200, 200);

				if (colorTest.isBackAndWhite(example.image))
					continue;

				compute(example);
			}
		}

	private:
		void compute(Reader::LearningExample& example)
		{
			auto normalizedAvgMask = MaskTransformations::MaskNormalizer::normalizeMask(
				avgMask,
				example.mask.faceCenter(),
				example.mask.faceSize());

			float interpolationFactors[]{
				0.,
				0.25,
				0.5,
				0.75,
				1.
			};

			for (auto factor : interpolationFactors)
			{
				auto currentMask = MaskTransformation::MaskInterpolation::interpolate(normalizedAvgMask, example.mask, factor);
				compute(example, currentMask);
			}
		}

		void compute(Reader::LearningExample& example, const FaceMask& currentMask)
		{
			featureSelector.setImage(example.image);

			for (size_t i = 0; i < example.mask.size(); i++)
			{
				std::vector<float> features;
				std::vector<float> decisions;

				featureSelector.selectFeatures(currentMask, i, features);
				FeatureExtraction::Decision::getDecisions(example.mask, currentMask, i, decisions);

				featuresIO.save(i, features, decisions);
			}
		}
	};
}