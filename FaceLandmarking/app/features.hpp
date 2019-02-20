#pragma once

#include "../learning/average-mask-processing.hpp"
#include "../learning/autoencoder-processing.hpp"
#include "../learning/regressor-processing.hpp"

namespace FaceLandmarking::App
{
	template<std::size_t N>
	void features(std::experimental::filesystem::path dataPath)
	{
		Data::Dataset<N> dataset(dataPath);

		auto begin = Data::DatasetMirroringIterator<N, Data::DatasetIterator<N>>(dataset.begin());
		auto end = Data::DatasetMirroringIterator<N, Data::DatasetIterator<N>>(dataset.end());

		//Learning::AverageMaskProcessing<N> averageMaskProcessing(dataPath);
		//averageMaskProcessing.compute(begin, end);

		Learning::RegressorProcessing<N> featureProcessing(dataPath);
		featureProcessing.compute(begin, end);

		/*Learning::AutoencoderProcessing<N> autoencoderProcessing(dataPath);
		autoencoderProcessing.compute(begin, end);*/
	}
}