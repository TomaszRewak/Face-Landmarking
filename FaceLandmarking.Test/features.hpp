#pragma once

#include "../FaceLandmarking.Learning/feature-processing.hpp"
#include "../FaceLandmarking.Learning/autoencoder-example-generator.hpp"
#include "ui/mask-ui.hpp"

using namespace cv;
using namespace std;
using namespace FaceLandmarking;

void features_test(experimental::filesystem::path dataPath)
{
	Learning::FeatureProcessing featureExtraction(dataPath);
	featureExtraction.compute();
}

void autoencoder_test(experimental::filesystem::path dataPath)
{
	Learning::AutoencoderExampleGenerator autoencoderExampleGenerator(dataPath);
	autoencoderExampleGenerator.compute();
}