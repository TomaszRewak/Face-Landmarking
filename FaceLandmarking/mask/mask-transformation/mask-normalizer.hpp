#pragma once

#include "../face-mask.hpp"

namespace FaceLandmarking::MaskTransformation::Mask
{
	template<size_t Nodes>
	class MaskNormalizer
	{
	public:
		MaskNormalizer(
			Math::Rect<float> sourceRect,
			Math::Rect<float> targetRect = Math::Rect<float>(Math::Point<float>(0, 0), Math::Size<float>(1, 1))
		) :
			sourceRect(sourceRect),
			targetRect(targetRect)
		{ }

		FaceMask<Nodes> operator()(const FaceMask<Nodes>& mask)
		{
			FaceMask<Nodes> normalizedMask = mask;

			float scaleFactor = std::min(
				targetRect.size.width / sourceRect.size.width,
				targetRect.size.height / sourceRect.size.height
			);

			auto maskOffset = Math::Point<float>(
				targetRect.center.x - sourceRect.center.x * scaleFactor,
				targetRect.center.y - sourceRect.center.y * scaleFactor
				);

			for (auto& point : normalizedMask)
			{
				point.x = point.x * scaleFactor + maskOffset.x;
				point.y = point.y * scaleFactor + maskOffset.y;
			}

			return normalizedMask;
		}

	private:
		Math::Rect<float> sourceRect;
		Math::Rect<float> targetRect;
	};
}