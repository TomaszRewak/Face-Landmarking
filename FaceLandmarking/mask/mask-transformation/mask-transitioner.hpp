#pragma once

#include "../face-mask.hpp"

namespace FaceLandmarking::Mask::MaskTransformation
{
	template<size_t Nodes>
	class MaskTransitioner
	{
	public:
		MaskTransitioner(const Math::Vector<float>& offset) :
			offset(offset)
		{ }

		FaceMask<Nodes> operator()(const FaceMask<Nodes>& mask)
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