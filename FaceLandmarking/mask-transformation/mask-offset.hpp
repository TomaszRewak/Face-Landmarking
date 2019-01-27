#pragma once

#include <vector>

#include "../math/vector.hpp"

namespace FaceLandmarking::MaskTransformation
{
	template<size_t N>
	class MaskOffset
	{
	private:
		std::array<Math::Vector<float>, N> pointOffsets;

	public:
		const Math::Vector<float>& operator[](size_t at) const
		{
			return pointOffsets[at];
		}

		Math::Vector<float>& operator[](size_t at)
		{
			return pointOffsets[at];
		}

		auto begin() const
		{
			return pointOffsets.begin();
		}

		auto begin()
		{
			return pointOffsets.begin();
		}

		auto end() const
		{
			return pointOffsets.end();
		}

		auto end()
		{
			return pointOffsets.end();
		}
	};
}

