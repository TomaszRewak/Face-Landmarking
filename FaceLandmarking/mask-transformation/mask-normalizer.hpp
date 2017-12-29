#pragma once

#include "../face-mask.hpp"

namespace FaceLandmarking::MaskTransformation
{
	class MaskNormalizer
	{
	public:
		static FaceMask normalizeMask(const FaceMask& mask, Math::Point<float> center, Math::Size<float> size)
		{
			FaceMask normalizedMask = mask;

			auto maskSize = mask.faceSize();
			float scaleFactor = std::min(
				size.width / maskSize.width,
				size.height / maskSize.height
			);

			auto maskCenter = mask.faceCenter();
			auto maskOffset = Math::Point<float>(
				center.x - maskCenter.x * scaleFactor,
				center.y - maskCenter.y * scaleFactor
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
			return normalizeMask(mask, Math::Point<float>(0, 0), Math::Size<float>(1, 1));
		}
	};
}