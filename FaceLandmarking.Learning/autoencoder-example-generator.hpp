#pragma once

#include <filesystem>
#include <random>

#include "../FaceLandmarking.Reader/dataset-reader.hpp"
#include "../FaceLandmarking.Reader/autoencoder-example-io.hpp"
#include "mask/threshold-builder.hpp"
#include "../FaceLandmarking/mask-transformation/mask-normalizer.hpp"
#include "../FaceLandmarking/mask-info/mask-description.hpp"
#include "../FaceLandmarking/mask-info/mask-limits.hpp"
#include "../FaceLandmarking/math/angle.hpp"

namespace FaceLandmarking::Learning
{
	namespace fs = std::experimental::filesystem;

	class AutoencoderExampleGenerator
	{
	private:
		fs::path dataPath;

	public:
		AutoencoderExampleGenerator(fs::path dataPath) :
			dataPath(dataPath)
		{ }

		void compute()
		{
			Reader::AutoencoderExampleIO io{};

			auto path = dataPath / "autoencoder" / "examples";
			fs::remove(path);

			io.open(path);

			Reader::DatasetReader reader(dataPath);
			while (reader.hasNext())
			{
				auto example = reader.loadNext(false);
				auto mask = example.mask;
				auto normalizedMask = MaskTransformation::MaskNormalizer::normalizeMask(mask);

				io.add(normalizedMask, normalizedMask);

				for (int i = 0; i < 4; i++) {
					auto newMask = addRandomNoise(normalizedMask, 0.1);
					io.add(normalizedMask, newMask);
				}
				for (int i = 0; i < 4; i++) {
					auto newMask = addRandomNoise(normalizedMask, 0.01);
					io.add(normalizedMask, newMask);
				}
				for (int i = 0; i < 4; i++) {
					auto newMask = addRandomNoise(normalizedMask, 0.001);
					io.add(normalizedMask, newMask);
				}
			}

			io.close();
		}

		FaceMask addRandomNoise(FaceMask mask, float grain)
		{
			std::default_random_engine e{};
			std::normal_distribution<float> normal_distribution{ 0, grain };

			for (auto& point : mask)
			{
				point.x += normal_distribution(e);
				point.y += normal_distribution(e);
			}

			return mask;
		}
	};
}