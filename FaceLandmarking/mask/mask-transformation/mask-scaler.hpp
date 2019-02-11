#pragma once

#include "../face-mask.hpp"

namespace FaceLandmarking::MaskTransformation::Mask
{
	template<size_t Nodes>
	class MaskScaler
	{
	public:
		MaskScaler(float scaleX, float scaleY, Math::Point<float> center) :
			scaleX(scaleX),
			scaleY(scaleY),
			center(center)
		{ }

		static FaceMask<Nodes> operator()(const FaceMask<Nodes>& mask)
		{
			FaceMask<Nodes> scaledMask = mask;

			for (auto& point : scaledMask)
			{
				point.x = center.x + (point.x - center.x) * scaleX;
				point.y = center.y + (point.y - center.y) * scaleY;
			}

			return scaledMask;
		}

	private:
		float scaleX;
		float scaleY;
		Math::Point<float> center;
	};
}