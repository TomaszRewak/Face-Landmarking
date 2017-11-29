#pragma once

#include <filesystem>

#include "../FaceLandmarking.Reader/dataset-reader.hpp"
#include "../FaceLandmarking.Reader/mask-io.hpp"
#include "../FaceLandmarking/mask-transformation/mask-avg.hpp"
#include "../FaceLandmarking/mask-transformation/mask-normalizer.hpp"

namespace FaceLandmarking::Learning
{
	namespace fs = std::experimental::filesystem;

	class AverageFace
	{
	private:
		fs::path dataPath;
		fs::path maskFile;

	public:
		AverageFace(fs::path dataPath) :
			dataPath(dataPath),
			maskFile(dataPath / "temp" / "avg-face.mask")
		{ }

		void compute() const
		{
			MaskTransformations::AverageMask averageMaskBuilder;
			Reader::DatasetReader reader(dataPath);

			while (reader.hasNext())
			{
				auto example = reader.loadNext(false);
				auto mask = example.mask;

				mask = MaskTransformations::MaskNormalizer::normalizeMask(mask);
				averageMaskBuilder.addMask(mask);
			}

			FaceMask averageMask = averageMaskBuilder.getAvg();

			Reader::MaskIO maskIO(maskFile);
			maskIO.save(averageMask);
		}

		FaceMask load() const
		{
			if (!fs::exists(maskFile))
				compute();

			Reader::MaskIO maskIO(maskFile);
			return maskIO.load();
		}
	};
}