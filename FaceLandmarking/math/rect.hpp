#pragma once

#include "point.hpp"
#include "size.hpp"
#include "offset.hpp"

namespace FaceLandmarking::Math
{
	template<typename T>
	struct Rect
	{
		Point<T> center;
		Size<T> size;

		Rect()
		{ }

		Rect(float topLeftX, float topLeftY, float width, float height) :
			center(topLeftX + width / 2, topLeftY + height / 2),
			size(width, height)
		{ }

		Rect(Point<T> center, Size<T> size) :
			center(center),
			size(size)
		{ }

		float left() const
		{
			return center.x - size.width / 2;
		}

		float right() const
		{
			return center.x + size.width / 2;
		}

		float top() const
		{
			return center.y - size.height / 2;
		}

		float down() const
		{
			return center.y + size.height / 2;
		}

		void operator+=(const Math::Offset<float>& offset)
		{
			center.x += (offset.right - offset.left) / 2;
			center.y += (offset.down - offset.top) / 2;

			size.width += offset.left + offset.right;
			size.height += offset.top + offset.down;
		}

		Rect<float> operator+(const Math::Offset<float>& offset) const
		{
			Rect<float> result = *this;

			result += offset;

			return result;
		}
	};
}