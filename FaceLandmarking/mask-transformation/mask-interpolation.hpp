#pragma once

#include "../face-mask.hpp"

namespace FaceLandmarking::MaskTransformation
{
	class MaskInterpolation
	{
	public:
		static FaceMask interpolate(const FaceMask& from, const FaceMask& to, float factor, bool xReflect = false, bool yReflect = false)
		{
			FaceMask interpolated = from;

			for (size_t i = 0; i < from.size(); i++)
			{
				auto v = from[i].to(to[i]);

				if (xReflect)
					v.x = -v.x;
				if (yReflect)
					v.y = -v.y;

				interpolated[i] = interpolated[i] + v * factor;
			}

			return interpolated;
		}
	};
}