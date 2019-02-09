#pragma once

#include "../face-mask.hpp"

namespace FaceLandmarking::MaskTransformation
{
	template<size_t N>
	class MaskTransitioner
	{
	public:
		MaskTransitioner(const Math::Vector<float>& offset) :
			offset(offset)
		{ }

		FaceMask<N> operator()(const FaceMask<N>& mask)
		{
			FaceMask movedMask = mask;

			for (auto& point : movedMask)
				point += offset;

			return movedMask;
		}

	private:
		Math::Vector<float> offset;
	};
}