#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <vector>
#include <filesystem>

#include "regressors/tree-mask-regressor.hpp"
#include "../math/point.hpp"
#include "../feature-extraction/hsv-image.hpp"
#include "../feature-extraction/feature-extractor.hpp"

namespace FaceLandmarking::Regression
{
	namespace fs = std::experimental::filesystem;

	class MaskRegressorBuffer
	{
	private:
		cv::Mat bufferComputed;
		cv::Mat bufferValues;

		std::vector<Math::Point<int>> setPoints;

	public:
		void resize(int rows, int cols)
		{
			if (rows > bufferValues.rows || cols > bufferValues.cols)
				bufferValues.create(
					std::max(rows, bufferValues.rows),
					std::max(cols, bufferValues.cols),
					CV_64F);
			if (rows > bufferComputed.rows || cols > bufferComputed.cols)
			{
				bufferComputed.create(
					std::max(rows, bufferComputed.rows),
					std::max(cols, bufferComputed.cols),
					CV_8U);
				bufferComputed *= 0;
				setPoints.clear();
			}
		}

		void clear()
		{
			for (auto& point : setPoints)
				bufferComputed.at<uchar>(point.y, point.x) = 0;

			setPoints.clear();
		}

		bool hasValue(int x, int y)
		{
			return bufferComputed.at<uchar>(y, x);
		}

		Math::Vector<float> getValue(int x, int y)
		{
			return bufferValues.at<Math::Vector<float>>(y, x);
		}

		void setValue(int x, int y, Math::Vector<float> offset)
		{
			bufferValues.at<Math::Vector<float>>(y, x) = offset;

			if (!hasValue(x, y))
			{
				bufferComputed.at<uchar>(y, x) = 1;
				setPoints.push_back(Math::Point<int>(x, y));
			}
		}
	};

	template<size_t N>
	class MaskRegressor
	{
	private:
		Regression::Regressors::TreeMaskRegressor<N> regressors;

		MaskRegressorBuffer buffer;

	public:
		MaskRegressor(fs::path dataPath) :
			regressors(dataPath / "regressors" / "trees")
		{ }

		Math::Vector<float> computeOffset(const FeatureExtraction::HsvImage& image, Math::Point<float> point, size_t pointNumber, size_t iterations)
		{
			FeatureExtraction::FeatureExtractor featureExtractor(image);

			int cols = image.columns();
			int rows = image.rows();

			buffer.resize(rows, cols);
			buffer.clear();

			Math::Vector<float> globalOffset;

			for (int i = 0; i < iterations; i++)
			{
				Math::Point<float> currentPosition = point + globalOffset;
				Math::Vector<float> localOffset;

				int xi = std::max(0, std::min((int)currentPosition.x, cols - 1));
				int yi = std::max(0, std::min((int)currentPosition.y, rows - 1));

				if (buffer.hasValue(xi, yi))
				{
					localOffset = buffer.getValue(xi, yi);
				}
				else
				{
					auto features = featureExtractor(Math::Point<int>(xi, yi));

					localOffset = regressors.getOffset(pointNumber, features);

					buffer.setValue(xi, yi, localOffset);
				}

				globalOffset += localOffset;
			}

			return globalOffset;
		}
	};
}