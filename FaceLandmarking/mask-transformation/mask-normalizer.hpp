#pragma once

#include "../face-mask.hpp"
#include "../math/rect.hpp"

namespace FaceLandmarking::MaskTransformation
{
	template<size_t N>
	class MaskNormalizer
	{
	public:
		static FaceMask<N> normalizeMask(const FaceMask<N>& mask, Math::Rect<float> sourceRect, Math::Rect<float> targetRect)
		{
			FaceMask<N> normalizedMask = mask;

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

		static FaceMask<N> normalizeMask(const FaceMask<N>& mask, Math::Rect<float> targetRect)
		{
			auto maskRect = mask.faceRect();

			return normalizeMask(mask, maskRect, targetRect);
		}

		static FaceMask<N> normalizeMask(const FaceMask<N>& mask)
		{
			return normalizeMask(mask, Math::Rect<float>(Math::Point<float>(0, 0), Math::Size<float>(1, 1)));
		}
	};
}