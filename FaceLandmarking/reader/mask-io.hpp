#pragma once

#include <fstream>

#include "../FaceLandmarking/face-mask.hpp"

namespace FaceLandmarking::Reader
{
	namespace fs = std::experimental::filesystem;

	template<size_t N>
	class MaskIO
	{
	private:
		fs::path path;

	public:
		MaskIO(fs::path path) :
			path(path)
		{ }

		void save(const FaceMask<N>& mask) const
		{
			std::fstream file;
			file.open(path, std::ofstream::out);

			file << N << std::endl;

			for (const auto& point : mask)
				file << point.x << " " << point.y << std::endl;

			file.close();
		}

		FaceMask<N> load() const
		{
			FaceMask<N> mask;

			std::fstream file;
			file.open(path, std::ofstream::in);

			int size;
			file >> size;

			for (int i = 0; i < size; i++)
			{
				Math::Point<float> point;
				file >> point.x >> point.y;

				mask[i] = point;
			}

			file.close();

			return mask;
		}
	};
}