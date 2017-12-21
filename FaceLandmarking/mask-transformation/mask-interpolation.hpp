#pragma once

#include "../face-mask.hpp"

namespace FaceLandmarking::MaskTransformation
{
	class MaskInterpolation
	{
	public:
		static FaceMask interpolate(const FaceMask& from, const FaceMask& to, float factor)
		{
			FaceMask interpolated = from;

			for (size_t i = 0; i < from.size(); i++)
			{
				interpolated[i].x += (to[i].x - from[i].x) * factor;
				interpolated[i].y += (to[i].y - from[i].y) * factor;
			}

			return interpolated;
		}
	};
}