#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <vector>
#include <filesystem>

#include "../FaceLandmarking/mask-transformation/mask-offset.hpp"
#include "../FaceLandmarking.FeatureExtraction/hsv-image.hpp"

namespace FaceLandmarking::Learning
{
	namespace fs = std::experimental::filesystem;

	template<typename FeatureExtractor, typename Regressor>
	class MaskRegression
	{
	private:
		MaskInfo::MaskDescription maskDescription;
		FeatureExtractor featureExtractor;
		Regressor regressors;

		std::vector<float> features;
		MaskTransformation::MaskOffset maskOffset;

		cv::Mat bufferComputed;
		cv::Mat bufferValues;

	public:
		MaskRegression(MaskInfo::MaskDescription maskDescription, Regressor regressors) :
			maskDescription(maskDescription),
			regressors(regressors)
		{ }

		void setImage(const FeatureExtraction::HsvImage& image)
		{
			featureExtractor.setImage(image);

			bufferValues.create(image.rows(), image.columns(), CV_64F);
			bufferComputed.create(image.rows(), image.columns(), CV_8U);

			for (int x = 0; x < bufferComputed.cols; x++)
				for (int y = 0; y < bufferComputed.rows; y++)
					bufferComputed.at<uchar>(y, x) = 0;
		}

		void compute(FaceMask& mask, int size = 2)
		{
			maskOffset.clear();
			maskOffset.resize(mask.size());

			for (size_t i = 0; i < mask.size(); i++)
			{
				if (!maskDescription.points[i].inUse)
					continue;

				Math::Vector<float> pointOffset;
				float factor = 0;

				for (int x = -size; x <= size; x++)
				{
					for (int y = -size; y <= size; y++)
					{
						Math::Vector<float> localOffset;

						int xi = std::max(0, std::min((int)mask[i].x + x, bufferComputed.cols - 1));
						int yi = std::max(0, std::min((int)mask[i].y + y, bufferComputed.rows - 1));
						if (bufferComputed.at<uchar>(yi, xi))
						{
							localOffset = bufferValues.at<Math::Vector<float>>(yi, xi);
						}
						else
						{
							features.clear();
							featureExtractor.selectFeatures(xi, yi, features);

							localOffset = regressors.getOffset(i, features);

							bufferValues.at<Math::Vector<float>>(yi, xi) = localOffset;
							bufferComputed.at<uchar>(yi, xi) = 1;
						}

						float localFactor = 1. / (std::abs(x) + std::abs(y) + 2);
						pointOffset += localOffset * localFactor;
						factor += localFactor;
					}
				}

				pointOffset = pointOffset / factor;

				maskOffset[i] = pointOffset;
			}
		}

		void apply(FaceMask& mask, float factor = 1)
		{
			for (size_t i = 0; i < mask.size(); i++)
				mask[i] += maskOffset[i] * factor;
		}
	};
}