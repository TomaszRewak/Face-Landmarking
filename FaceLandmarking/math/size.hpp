#pragma once

namespace FaceLandmarking::Math
{
	template<typename T>
	struct Size
	{
		T width;
		T height;

		Size() :
			width(0),
			height(0)
		{ }

		Size(T width, T height) :
			width(width),
			height(height)
		{ }

		void operator*=(float by) 
		{
			width *= by;
			height *= by;
		}
	};
}