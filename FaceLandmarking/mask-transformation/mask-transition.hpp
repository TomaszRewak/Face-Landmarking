#pragma once

#include "../face-mask.hpp"

namespace FaceLandmarking::MaskTransformation
{
	class MaskTransition
	{
	public:
		static FaceMask moveMask(const FaceMask& mask, Math::Vector<float> offset)
		{
			FaceMask movedMask = mask;

			for (auto& point : movedMask)
				point += offset;

			return movedMask;
		}

		static FaceMask scale(const FaceMask& mask, float scale, Math::Point<float> center)
		{
			FaceMask scaledMask = mask;

			for (auto& point : scaledMask)
			{
				point.x = center.x + (point.x - center.x) * scale;
				point.y = center.y + (point.y - center.y) * scale;
			}

			return scaledMask;
		}
	};
}