#pragma once

#include "../face-mask.hpp"

namespace FaceLandmarking::MaskTransformation
{
	template<size_t N>
	class MaskTransition
	{
	public:
		static FaceMask<N> moveMask(const FaceMask<N>& mask, Math::Vector<float> offset)
		{
			FaceMask movedMask = mask;

			for (auto& point : movedMask)
				point += offset;

			return movedMask;
		}

		static FaceMask<N> scale(const FaceMask<N>& mask, float scaleX, float scaleY, Math::Point<float> center)
		{
			FaceMask<N> scaledMask = mask;

			for (auto& point : scaledMask)
			{
				point.x = center.x + (point.x - center.x) * scaleX;
				point.y = center.y + (point.y - center.y) * scaleY;
			}

			return scaledMask;
		}

		static FaceMask<N> scale(const FaceMask<N>& mask, float scaleX, float scaleY)
		{
			return scale(mask, scaleX, scaleY, mask.faceCenter());
		}
	};
}