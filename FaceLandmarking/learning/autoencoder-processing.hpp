#pragma once

#include <filesystem>
#include <random>

#include "../data/dataset.hpp"
#include "../data/dataset-mirroring-iterator.hpp"
#include "../io/autoencoder-example-io.hpp"

namespace FaceLandmarking::Learning
{
	namespace fs = std::experimental::filesystem;

	template<size_t N>
	class AutoencoderProcessing
	{
	private:
		fs::path dataPath;
		std::default_random_engine e{};

	public:
		AutoencoderProcessing(fs::path dataPath) :
			dataPath(dataPath)
		{ }

		template<typename DatasetIterator>
		void compute(DatasetIterator begin, DatasetIterator end)
		{
			IO::AutoencoderExampleIO<N> io;

			auto path = dataPath / "autoencoder" / "examples";
			fs::remove(path);

			io.open(path);

			for (auto iter = begin; iter != end; iter++)
			{
				auto example = *iter;
				auto mask = example.mask;

				auto normalizedMask = Mask::MaskTransformation::MaskNormalizer<N>(mask.faceRect())(mask);
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

		Mask::FaceMask<N> addRandomNoise(Mask::FaceMask<N> mask, float grain)
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