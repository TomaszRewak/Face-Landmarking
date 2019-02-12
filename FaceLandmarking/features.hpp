#pragma once

#include "ui/mask-ui.hpp"

using namespace cv;
using namespace std;
using namespace FaceLandmarking;

template<size_t N>
void features_test(experimental::filesystem::path dataPath)
{
	using DataSetReader = Reader::DatasetReader<N>;

	Learning::FeatureProcessing<N, DataSetReader> featureExtraction(dataPath);
	featureExtraction.compute();
}

template<size_t N>
void autoencoder_test(experimental::filesystem::path dataPath)
{
	using DataSetReader = Reader::DatasetReader<N>;

	Learning::AutoencoderExampleGenerator<N, DataSetReader> autoencoderExampleGenerator(dataPath);
	autoencoderExampleGenerator.compute();
}