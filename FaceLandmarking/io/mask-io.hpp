#pragma once

#include <fstream>

#include "../mask/face-mask.hpp"

namespace FaceLandmarking::IO
{
	namespace fs = std::experimental::filesystem;

	template<size_t N>
	class MaskIO
	{
	public:
		static void save(fs::path path, const Mask::FaceMask<N>& mask)
		{
			std::fstream file;
			file.open(path, std::ofstream::out);

			file << N << std::endl;

			for (const auto& point : mask)
				file << point.x << " " << point.y << std::endl;

			file.close();
		}

		static Mask::FaceMask<N> load(fs::path path)
		{
			Mask::FaceMask<N> mask;

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