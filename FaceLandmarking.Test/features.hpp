#pragma once

#include "../FaceLandmarking.Learning/feature-processing.hpp"
#include "../FaceLandmarking.Learning/autoencoder-example-generator.hpp"
#include "ui/mask-ui.hpp"

using namespace cv;
using namespace std;
using namespace FaceLandmarking;

template<size_t N>
void features_test(experimental::filesystem::path dataPath)
{
	Learning::FeatureProcessing<N> featureExtraction(dataPath);
	featureExtraction.compute();
}

template<size_t N>
void autoencoder_test(experimental::filesystem::path dataPath)
{
	Learning::AutoencoderExampleGenerator<N> autoencoderExampleGenerator(dataPath);
	autoencoderExampleGenerator.compute();
}