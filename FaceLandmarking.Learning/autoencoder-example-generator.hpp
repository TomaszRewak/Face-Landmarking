#pragma once

#include <filesystem>
#include <random>

#include "../FaceLandmarking.Reader/dataset-reader.hpp"
#include "../FaceLandmarking.Reader/autoencoder-example-io.hpp"
#include "../FaceLandmarking/mask-transformation/mask-normalizer.hpp"
#include "../FaceLandmarking/math/angle.hpp"

namespace FaceLandmarking::Learning
{
	namespace fs = std::experimental::filesystem;

	template<size_t N, typename DatasetReader>
	class AutoencoderExampleGenerator
	{
	private:
		fs::path dataPath;
		std::default_random_engine e{};

	public:
		AutoencoderExampleGenerator(fs::path dataPath) :
			dataPath(dataPath)
		{ }

		void compute()
		{
			Reader::AutoencoderExampleIO<N> io{};

			auto path = dataPath / "autoencoder" / "examples";
			fs::remove(path);

			io.open(path);

			DatasetReader reader(dataPath);
			while (reader.hasNext())
			{
				auto example = reader.loadNext(false);
				auto mask = example.mask;

				auto normalizedMask = MaskTransformation::MaskNormalizer<N>::normalizeMask(mask);
				auto normalizedMaskRect = normalizedMask.faceRect();

				io.add(normalizedMask, normalizedMask);

				for (float grain : {0.05, 0.01, 0.001})
				{
					for (int i = 0; i < 4; i++) {
						auto newMask = addRandomNoise(normalizedMask, grain);
						auto newMaskRect = newMask.faceRect();

						io.add(
							MaskTransformation::MaskNormalizer<N>::normalizeMask(newMask, newMaskRect, normalizedMaskRect),
							MaskTransformation::MaskNormalizer<N>::normalizeMask(normalizedMask, newMaskRect, normalizedMaskRect)
						);
					}
				}
			}

			io.close();
		}

		FaceMask<N> addRandomNoise(FaceMask<N> mask, float grain)
		{
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