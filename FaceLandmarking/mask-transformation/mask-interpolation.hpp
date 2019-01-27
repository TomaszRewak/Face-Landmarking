#pragma once

#include "../face-mask.hpp"

namespace FaceLandmarking::MaskTransformation
{
	template<size_t N>
	class MaskInterpolation
	{
	public:
		static FaceMask<N> interpolate(const FaceMask<N>& from, const FaceMask<N>& to, float factor, bool xReflect = false, bool yReflect = false)
		{
			FaceMask<N> interpolated = from;

			for (size_t i = 0; i < N; i++)
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