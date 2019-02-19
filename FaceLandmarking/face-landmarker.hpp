#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <filesystem>

#include "data/dataset.hpp"
#include "preprocessing/face-finder.hpp"
#include "preprocessing/mask-frame.hpp"
#include "regression/mask-regressor.hpp"
#include "regression/mask-autoencoder.hpp"
#include "regression/regressors/nn-regressor.hpp"
#include "regression/regressors/tree-mask-regressor.hpp"
#include "feature-extraction/feature-extractor.hpp"
#include "feature-extraction/image-preprocessor.hpp"
#include "mask/mask-transformation/mask-scaler.hpp"

namespace FaceLandmarking
{
	namespace fs = std::experimental::filesystem;

	template<size_t N>
	class FaceLandmarker
	{
	private:
		Mask::FaceMask<N> averageMask;
		Preprocessing::MaskFrame<N> maskFrame;

		Preprocessing::FaceFinder faceFinder;
		FeatureExtraction::ImagePreprocessor imagePreprocessor;

		Regression::MaskRegressor<N> maskRegression;
		Regression::MaskAutoencoder<N> maskAutoencoder;

	public:
		FaceLandmarker(fs::path dataPath) :
			averageMask(IO::MaskIO<N>::load(dataPath / "mask" / "avg-face.mask")),
			maskFrame(IO::MaskIO<N>::load(dataPath / "mask" / "avg-face.mask"), Math::Size<float>(150, 150)),
			faceFinder(dataPath / "haar" / "haarcascade_frontalface_default.xml"),
			maskRegression(dataPath),
			maskAutoencoder(dataPath)
		{ }

		std::vector<Mask::FaceMask<N>> masks;

		void findFaces(const cv::Mat& frame)
		{
			masks.clear();

			for (auto rect : faceFinder.locate(frame))
				masks.push_back(Mask::MaskTransformation::MaskNormalizer<N>(averageMask.faceRect(), rect)(averageMask));
		}

		void adjustMasks(const cv::Mat& frame, int steps)
		{
			for (auto& mask : masks)
				adjustMasks(frame, mask, steps);
		}

	private:
		cv::Mat scaledFrame;
		FeatureExtraction::HsvImage processedFrame;

		void adjustMasks(const cv::Mat& frame, Mask::FaceMask<N>& mask, int steps)
		{
			auto scale = maskFrame.getScale(mask);
			auto normalizedMask = Mask::MaskTransformation::MaskScaler<N>(scale, scale, Math::Point<float>(0, 0))(mask);

			cv::resize(frame, scaledFrame, cv::Size(frame.cols * scale, frame.rows * scale));

			auto faceRect = maskFrame.getFrame(mask);
			auto normalizedFaceRect = maskFrame.getFrame(normalizedMask);

			imagePreprocessor.processImage(scaledFrame, processedFrame, normalizedFaceRect * 0.5, true);

			for (int i = 0; i < N; i++)
			{
				mask[i] += maskRegression.computeOffset(processedFrame, normalizedMask[i], i, steps) / scale;
			}

			mask = maskAutoencoder(maskAutoencoder(mask));
		}
	};
}