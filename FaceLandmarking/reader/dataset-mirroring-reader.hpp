#pragma once

#include "dataset-reader.hpp"

namespace FaceLandmarking::Reader
{
	namespace fs = std::experimental::filesystem;

	template<typename BaseReader>
	class DatasetMirroringReader
	{
	private:
		BaseReader datasetReader;


	public:
		DatasetMirroringReader(fs::path path) : datasetReader(path)
		{ }

		bool hasNext() const
		{
			return datasetReader.hasNext();
		}

		LearningExample<BaseReader::Nodes> loadNext(bool includeImage = true)
		{
			LearningExample<BaseReader::Nodes> fullExample = datasetReader.loadNext(includeImage);
			FaceMask<Output_N> reducedMask;

			for (size_t i = 0; i < Output_N; i++)
				reducedMask[i] = fullExample.mask[pointMapping[i]];

			return LearningExample<Output_N>(fullExample.image, reducedMask);
		}
	};
}