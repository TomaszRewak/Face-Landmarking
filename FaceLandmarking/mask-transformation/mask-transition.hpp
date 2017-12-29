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
	};
}