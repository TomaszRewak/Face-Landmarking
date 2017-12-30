#pragma once

#include <vector>
#include <filesystem>

#include "../FaceLandmarking/mask-transformation/mask-offset.hpp"

namespace FaceLandmarking::Learning
{
	namespace fs = std::experimental::filesystem;

	template<typename FeatureExtractor, typename Regressor>
	class MaskRegression
	{
	private:
		MaskInfo::MaskDescription& maskDescription;
		FeatureExtractor & featureExtractor;
		Regressor& regressors;

		std::vector<float> features;
		MaskTransformation::MaskOffset maskOffset;

	public:
		MaskRegression(MaskInfo::MaskDescription& maskDescription, FeatureExtractor& featureExtractor, Regressor& regressors) :
			maskDescription(maskDescription),
			featureExtractor(featureExtractor),
			regressors(regressors)
		{ }

		void adjust(FaceMask& mask)
		{
			maskOffset.clear();
			maskOffset.resize(mask.size());

			for (size_t i = 0; i < mask.size(); i++)
			{
				if (!maskDescription.points[i].inUse)
					continue;

				Math::Vector<float> pointOffset;
				float factor = 0;

				for (int x = -1; x <= 1; x++)
					for (int y = -1; y <= 1; y++)
					{
						features.clear();
						featureExtractor.selectFeatures(mask[i].x + x, mask[i].y + y, features);

						float localFactor = 1. / (std::abs(x) + std::abs(y) + 2);
						pointOffset += regressors.getOffset(i, features) * localFactor;
						factor += localFactor;
					}

				pointOffset = pointOffset / factor;

				maskOffset[i] = pointOffset;
			}

			mask += maskOffset;
		}
	};
}