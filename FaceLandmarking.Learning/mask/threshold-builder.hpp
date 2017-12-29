#pragma once

#include "../../FaceLandmarking/mask-info/mask-limits.hpp"

namespace FaceLandmarking::Learning::Mask
{
	class ThresholdBuilder
	{
	private:
		std::vector<float> values;
		int thresholdsNumber;

	public:
		ThresholdBuilder(int thresholdsNumber):
			thresholdsNumber(thresholdsNumber)
		{ }

		void addValue(float value)
		{
			values.push_back(value);
		}

		MaskInfo::Threshold getThreshold()
		{
			std::sort(values.begin(), values.end());

			MaskInfo::Threshold threshold;

			for (int i = 0; i < thresholdsNumber; i++)
			{
				float value = values[(values.size() - 1) * i / (thresholdsNumber - 1)];

				if (value == std::numeric_limits<float>::infinity())
					value = std::numeric_limits<float>::max();

				if (value == -std::numeric_limits<float>::infinity())
					value = std::numeric_limits<float>::min();

				threshold.push_back(value);
			}
			
			return threshold;
		}
	};
}