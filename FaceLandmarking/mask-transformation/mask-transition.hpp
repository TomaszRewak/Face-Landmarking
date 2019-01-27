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

		static FaceMask scale(const FaceMask& mask, float scaleX, float scaleY, Math::Point<float> center)
		{
			FaceMask scaledMask = mask;

			for (auto& point : scaledMask)
			{
				point.x = center.x + (point.x - center.x) * scaleX;
				point.y = center.y + (point.y - center.y) * scaleY;
			}

			return scaledMask;
		}

		static FaceMask scale(const FaceMask& mask, float scaleX, float scaleY)
		{
			return scale(mask, scaleX, scaleY, mask.faceCenter());
		}
	};
}