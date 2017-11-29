#pragma once

#include <filesystem>

#include "average-face.hpp"
#include "decision.hpp"
#include "../FaceLandmarking.Reader/features-io.hpp"
#include "../FaceLandmarking.FeartureSelection/feature-selector.hpp"

namespace FaceLandmarking::Learning
{
	namespace fs = std::experimental::filesystem;

	class FeatureExtraction
	{
	private:
		fs::path dataPath;
		FeatureSelection::FeatureSelector featureSelector;

	public:
		FeatureExtraction(fs::path dataPath) :
			dataPath(dataPath)
		{ }

		void compute()
		{
			AverageFace averageFaceLoader(dataPath);
			auto averageFace = averageFaceLoader.load();

			std::vector<float> features;
			std::vector<float> decisions;

			Reader::FeaturesIO featuresIO(dataPath / "features");
			Reader::DatasetReader reader(dataPath);

			while (reader.hasNext())
			{
				auto[image, face] = reader.loadNext(true);
				featureSelector.setImage(image);

				for (size_t i = 0; i < face.size(); i++)
				{
					features.clear();
					decisions.clear();

					featureSelector.selectFeatures(averageFace, i, features);
					Decision::getDecisions(face, averageFace, i, decisions);

					featuresIO.save(i, features, decisions);
				}
			}
		}
	};
}