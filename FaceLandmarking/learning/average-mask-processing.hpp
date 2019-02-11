#pragma once

#include <filesystem>

#include "../FaceLandmarking.Reader/dataset-reader.hpp"
#include "../FaceLandmarking.Reader/mask-io.hpp"
#include "mask/mask-avg.hpp"
#include "../FaceLandmarking/mask-transformation/mask-normalizer.hpp"

namespace FaceLandmarking::Learning
{
	namespace fs = std::experimental::filesystem;

	template<size_t Nodes, typename DatasetReader>
	class AverageMaskProcessing
	{
	private:
		fs::path dataPath;
		fs::path maskFile;

	public:
		AverageMaskProcessing(fs::path dataPath) :
			dataPath(dataPath),
			maskFile(dataPath / "mask" / "avg-face.mask")
		{ }

		void compute() const
		{
			Mask::AverageMask<Nodes> averageMaskBuilder;
			DatasetReader reader(dataPath);

			while (reader.hasNext())
			{
				auto example = reader.loadNext(false);
				auto mask = example.mask;

				mask = MaskTransformation::MaskNormalizer<N>::normalizeMask(mask);
				averageMaskBuilder.addMask(mask);
			}

			FaceMask<N> averageMask = averageMaskBuilder.getAvg();

			Reader::MaskIO<Nodes> maskIO(maskFile);
			maskIO.save(averageMask);
		}
	};
}