#pragma once

namespace FaceLandmarking::Math
{
	template<typename T>
	struct Offset
	{
		T left, right, top, down;

		Offset() :
			left(0),
			right(0),
			top(0),
			down(0)
		{ }

		Offset(T left, T right, T top, T down) :
			left(left),
			right(right),
			top(top),
			down(down)
		{ }

		void operator*=(T factor)
		{
			left *= factor;
			right *= factor;
			top *= factor;
			down *= factor;
		}

		Offset<T> operator*(T factor) const
		{
			Offset<T> result = *this;

			result *= factor;

			return result;
		}
	};
}