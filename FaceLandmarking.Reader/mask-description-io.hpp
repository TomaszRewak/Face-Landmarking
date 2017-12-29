#pragma once

#include <filesystem>
#include <fstream>

#include "../FaceLandmarking/mask-info/mask-description.hpp"

namespace FaceLandmarking::Reader
{
	namespace fs = std::experimental::filesystem;

	class MaskDescriptionIO
	{
	private:
		fs::path path;

	public:
		MaskDescriptionIO(fs::path path) :
			path(path)
		{ }

		MaskInfo::MaskDescription load() const
		{
			MaskInfo::MaskDescription maskDescription;

			std::fstream file;
			file.open(path, std::ofstream::in);

			for (bool read = true; read;)
			{
				std::string command;
				file >> command;

				if (command == "all_points")
				{
					int pointsNumber;
					file >> pointsNumber;
					maskDescription.points.resize(pointsNumber);
				}
				if (command == "floating_points_range")
				{
					size_t min, max;
					file >> min >> max;

					for (size_t i = min; i <= max; i++)
						maskDescription.points[i].floating = true;
				}
				if (command == "default_direction")
				{
					float x, y;
					file >> x >> y;

					size_t min, max;
					file >> min >> max;

					for (size_t i = min; i <= max; i++)
						maskDescription.points[i].defaultDirection = Math::Vector<float>(x, y);
				}
				else if (command == "shape")
					maskDescription.shapes.push_back(MaskInfo::MaskShape());
				else if (command == "name")
					file >> maskDescription.shapes.rbegin()->name;
				else if (command == "points_range")
				{
					size_t min, max;
					file >> min >> max;

					for (size_t i = min; i <= max; i++)
						maskDescription.shapes.rbegin()->points.push_back(i);
				}
				else if (command == "points")
				{
					size_t shapePointsNumber;
					file >> shapePointsNumber;

					for (size_t i = 0; i < shapePointsNumber; i++)
					{
						size_t point;
						file >> point;

						maskDescription.shapes.rbegin()->points.push_back(point);
					}
				}
				else if (command == "closed")
					file >> maskDescription.shapes.rbegin()->closed;
				else if (command == "visible")
					file >> maskDescription.shapes.rbegin()->visible;
				else if (command == "end")
					read = false;
			}

			for (auto& shape : maskDescription.shapes)
				for (auto& point : shape.points)
					maskDescription.points[point].inUse = true;

			file.close();

			return maskDescription;
		}
	};
}