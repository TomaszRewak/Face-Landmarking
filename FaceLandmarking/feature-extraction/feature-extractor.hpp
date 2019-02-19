#pragma once

#include "filter.hpp"
#include "hsv-image.hpp"
#include "../math/point.hpp"

namespace FaceLandmarking::FeatureExtraction
{
	class FeatureExtractor
	{
	public:
		static const std::size_t Length = Filter::Length * HsvChannels.size();

		FeatureExtractor(const HsvImage& hsv) :
			hsv(hsv)
		{ }

		std::array<float, Length> operator()(const Math::Point<int>& point)
		{
			std::array<float, Length> result;

			for (std::size_t i = 0; i < Length; i++)
				result[i] = operator()(point, i);

			return result;
		}

		float operator()(const Math::Point<int>& point, std::size_t feature)
		{
			return selectFeatures(
				point,
				HsvChannels[feature / Filter::Length],
				feature % Filter::Length);
		}

	private:
		float selectFeatures(const Math::Point<int>& point, HsvChannel channel, std::size_t filter)
		{
			return Filter(hsv[channel], filter)(point.x, point.y);
		}

		const HsvImage& hsv;
	};
}