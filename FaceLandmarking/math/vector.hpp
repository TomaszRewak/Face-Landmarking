#pragma once

namespace FaceLandmarking::Math
{
	template<typename T>
	struct Vector {
		T x;
		T y;

		Vector() :
			x(0),
			y(0)
		{ }

		Vector(T x, T y) :
			x(x),
			y(y)
		{ }

		Vector operator-() const
		{
			return Vector(-x, -y);
		}

		Vector operator+(const Vector& second) const
		{
			return Vector(x + second.x, y + second.y);
		}

		void operator+=(const Vector& second)
		{
			x += second.x;
			y += second.y;
		}

		Vector operator-(const Vector& second) const
		{
			return Vector(x - second.x, y - second.y);
		}

		void operator-=(const Vector& second)
		{
			x -= second.x;
			y -= second.y;
		}

		Vector operator/(T value) const
		{
			return Vector(x / value, y / value);
		}

		Vector operator*(T value) const
		{
			return Vector(x * value, y * value);
		}

		void operator*=(T value)
		{
			x *= value;
			y *= value;
		}

		T d() const
		{
			return std::sqrt(x * x + y * y);
		}

		Vector normal() const
		{
			return Vector(y, -x);
		}

		Vector normalized(float to = 1) const
		{
			float length = d();

			return Vector(x * to / length, y * to / length);
		}
	};
}