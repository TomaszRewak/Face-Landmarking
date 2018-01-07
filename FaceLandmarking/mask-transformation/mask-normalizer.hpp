#pragma once

#include "../face-mask.hpp"
#include "../math/rect.hpp"

namespace FaceLandmarking::MaskTransformation
{
	class MaskNormalizer
	{
	public:
		static FaceMask normalizeMask(const FaceMask& mask, Math::Rect<float> rect)
		{
			FaceMask normalizedMask = mask;

			auto maskSize = mask.faceSize();
			float scaleFactor = std::min(
				rect.size.width / maskSize.width,
				rect.size.height / maskSize.height
			);

			auto maskCenter = mask.faceCenter();
			auto maskOffset = Math::Point<float>(
				rect.center.x - maskCenter.x * scaleFactor,
				rect.center.y - maskCenter.y * scaleFactor
			);

			for (auto& point : normalizedMask)
			{
				point.x = point.x * scaleFactor + maskOffset.x;
				point.y = point.y * scaleFactor + maskOffset.y;
			}

			return normalizedMask;
		}

		static FaceMask normalizeMask(const FaceMask& mask)
		{
			return normalizeMask(mask, Math::Rect<float>(Math::Point<float>(0, 0), Math::Size<float>(1, 1)));
		}
	};
}