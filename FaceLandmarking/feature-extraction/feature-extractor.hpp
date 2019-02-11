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

		FeatureExtractor(const HsvImage& image) :
			hsv(hsv)
		{ }

		std::array<float, Length> operator()(const Math::Point<float>& point)
		{
			std::array<float, Length> result;

			for (std::size_t i = 0; i < Length; i++)
				result[i] = operator()(point, i);

			return result;
		}

		float operator()(const Math::Point<float>& point, std::size_t filter)
		{
			return selectFeatures(
				point,
				HsvChannels[filter / Filter::Length],
				filter / HsvChannels.size());
		}

	private:
		float selectFeatures(const Math::Point<float>& point, HsvChannel channel, std::size_t filter)
		{
			return Filter(hsv[channel], filter)(point.x, point.y);
		}

		const HsvImage& hsv;
	};
}