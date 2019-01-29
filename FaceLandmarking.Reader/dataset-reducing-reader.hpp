#pragma once

#include <filesystem>

#include "dataset-reader.hpp"

// --> jaw 0 40
// --> nose 41 57
// --> outer upper lip 58 71 (include edge)
// <-- outer lower lip 72 85
// --> inner upper lip 86 100 (include edge)
// <-- inner lower lip 101 113
// --> left upper eye 114 124
// <-- left lower eye 125 133
// <-- right upper eye 134 144
// --> right lower eye 145 153
// --> left upper eyebrow 154 164
// <-- left lower eyebrow 165 173
// <-- right upper eyebrow 174 184
// --> right lower eyebrow 185 193

namespace FaceLandmarking::Reader
{
	namespace fs = std::experimental::filesystem;

	template<typename BaseReader>
	class DatasetReducingReader
	{
	private:
		BaseReader datasetReader;

		static const size_t Input_N = 194;
		static const size_t Output_N = 68;

		const std::array<size_t, Output_N> pointMapping = {
			0,   5,   10,  15,  20,  25,  30,  35,  40,	// jaw
			41,  45,  49,  53,  57,						// nose
			58,  60,  63,  66,  69,  71,				// outer upper lip
			72,  74,  77,  80,  83,  85,				// outer lower lip
			86,  89,  92,  94,  97,  100,				// inner upper lip
			101, 103, 106, 108, 111, 113,				// inner lower lip
			114, 116, 118, 120, 122, 124,				// left upper eye
			125, 127, 129, 131, 133,					// left lower eye
			134, 136, 138, 140, 142, 144,				// right upper eye
			145, 147, 149, 151, 153,					// right lower eye
			//154, 159, 164,								// left upper eyebrow
			//169,										// left lower eyebrow
			//174, 179, 184,								// right upper eyebrow
			//189											// right lower eyebrow
		};

	public:
		DatasetReducingReader(fs::path path) : datasetReader(path)
		{ }

		bool hasNext() const
		{
			return datasetReader.hasNext();
		}

		LearningExample<Output_N> loadNext(bool includeImage = true)
		{
			LearningExample<Input_N> fullExample = datasetReader.loadNext(includeImage);
			FaceMask<Output_N> reducedMask;

			for (size_t i = 0; i < Output_N; i++)
				reducedMask[i] = fullExample.mask[pointMapping[i]];

			return LearningExample<Output_N>(fullExample.image, reducedMask);
		}
	};
}