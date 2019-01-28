#pragma once

#include "../FaceLandmarking.Learning/feature-processing.hpp"
#include "../FaceLandmarking.Learning/autoencoder-example-generator.hpp"
#include "ui/mask-ui.hpp"

using namespace cv;
using namespace std;
using namespace FaceLandmarking;

template<size_t N, size_t N_in>
void features_test(experimental::filesystem::path dataPath)
{
	using DataSetReader = Reader::DatasetReducingReader<Reader::DatasetReader<N_in>>;

	Learning::FeatureProcessing<N, DataSetReader> featureExtraction(dataPath);
	featureExtraction.compute();
}

template<size_t N, size_t N_in>
void autoencoder_test(experimental::filesystem::path dataPath)
{
	using DataSetReader = Reader::DatasetReducingReader<Reader::DatasetReader<N_in>>;

	Learning::AutoencoderExampleGenerator<N, DataSetReader> autoencoderExampleGenerator(dataPath);
	autoencoderExampleGenerator.compute();
}