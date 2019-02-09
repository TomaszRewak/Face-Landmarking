#pragma once

#include "../face-mask.hpp"

namespace FaceLandmarking::MaskTransformation
{
	template<size_t Nodes>
	class MaskInterpolator
	{
	public:
		MaskInterpolator(float factor, bool xReflect = false, bool yReflect = false) :
			factor(factor),
			xReflect(xReflect),
			yReflect(yReflect)
		{ }

		FaceMask<Nodes> operator()(const FaceMask<Nodes>& from, const FaceMask<Nodes>& to)
		{
			FaceMask<Nodes> interpolated = from;

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

	private:
		float factor;
		bool xReflect; 
		bool yReflect;
	};
}