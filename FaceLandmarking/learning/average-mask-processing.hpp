#pragma once

#include <filesystem>
#include <iostream>

#include "../io/mask-io.hpp"
#include "../mask/mask-transformation/mask-averager.hpp"
#include "../mask/mask-transformation/mask-normalizer.hpp"

namespace FaceLandmarking::Learning
{
	namespace fs = std::experimental::filesystem;

	template<size_t Nodes>
	class AverageMaskProcessing
	{
	private:
		fs::path maskFile;

	public:
		AverageMaskProcessing(fs::path dataPath) :
			maskFile(dataPath / "mask" / "avg-face.mask")
		{ }

		template<typename DatasetIterator>
		void compute(DatasetIterator begin, DatasetIterator end) const
		{
			std::cout << std::endl << "Average face processing";

			Mask::MaskTransformation::MaskAverager<Nodes> averageMaskBuilder;

			for (auto iter = begin; iter != end; ++iter)
			{
				auto example = *iter;
				auto mask = example.mask;

				mask = Mask::MaskTransformation::MaskNormalizer<Nodes>(mask.faceRect())(mask);
				averageMaskBuilder.addMask(mask);

				std::cout << ".";
			}

			Mask::FaceMask<Nodes> averageMask = averageMaskBuilder.getAvg();

			IO::MaskIO<Nodes>::save(maskFile, averageMask);
		}
	};
}