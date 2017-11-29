#pragma once

#include <fstream>

#include "../FaceLandmarking/face-mask.hpp"

namespace FaceLandmarking::Reader
{
	namespace fs = std::experimental::filesystem;

	class MaskIO
	{
	private:
		fs::path path;

	public:
		MaskIO(fs::path path) :
			path(path)
		{ }

		void save(const FaceMask& mask) const
		{
			std::fstream file;
			file.open(path, std::ofstream::out);

			file << mask.size() << std::endl;

			for (const auto& point : mask)
				file << point.x << " " << point.y << std::endl;

			file.close();
		}

		FaceMask load() const
		{
			FaceMask mask;

			std::fstream file;
			file.open(path, std::ofstream::in);

			int size;
			file >> size;

			for (int i = 0; i < size; i++)
			{
				Math::Point<float> point;
				file >> point.x >> point.y;

				mask.push_back(point);
			}

			file.close();

			return mask;
		}
	};
}