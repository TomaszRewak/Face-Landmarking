#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include "../data/dataset.hpp"

using namespace cv;
using namespace std;
using namespace FaceLandmarking;

template<size_t N>
void example_test(experimental::filesystem::path dataPath, std::size_t steps)
{
	Data::Dataset<N> dataset(dataPath);
	FaceLandmarking::FaceLandmarker<N> faceLandmarker(dataPath);
	Mask::FaceMask averageMask = IO::MaskIO<N>::load(dataPath / "mask" / "avg-face.mask");

	namedWindow("example", WINDOW_AUTOSIZE);

	Mat imageWithMasks;

	for (auto iter = dataset.begin(); iter != dataset.end(); ++iter)
	{
		auto example = *iter;
		example.scaleFace(200, 200);

		faceLandmarker.masks.clear();
		faceLandmarker.masks.push_back(Mask::MaskTransformation::MaskNormalizer<N>(Math::Rect<float>(example.mask.faceCenter(), example.mask.faceSize()))(averageMask));

		while (true)
		{
			faceLandmarker.adjustMasks(example.image, steps);

			example.image.copyTo(imageWithMasks);
			Test::UI::MaskUI<N>::drawMask(imageWithMasks, example.mask);
			Test::UI::MaskUI<N>::drawMask(imageWithMasks, faceLandmarker.masks[0], cv::Scalar(0, 0, 255));
			imshow("example", imageWithMasks);

			auto key = waitKey(250000);
			if (key == 32) // space
				continue;
			else if (key == 27) // escape
				return;
			else
				break;
		}
	}
}
