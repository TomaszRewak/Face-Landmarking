#pragma once

#include "../FaceLandmarking.Learning/feature-processing.hpp"
#include "ui/mask-ui.hpp"

using namespace cv;
using namespace std;
using namespace FaceLandmarking;

void features_test()
{
	auto dataPath = experimental::filesystem::path("D:\\Programy\\FaceLandmarking\\Data");

	Learning::FeatureProcessing featureExtraction(dataPath);
	featureExtraction.compute();
}
