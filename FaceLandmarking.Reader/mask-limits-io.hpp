#pragma once

#include <filesystem>
#include <fstream>

#include "../FaceLandmarking/mask-info/mask-limits.hpp"
#include "readers/threshold-reader.hpp"

namespace FaceLandmarking::Reader
{
	namespace fs = std::experimental::filesystem;

	class MaskLimitsIO
	{
	private:
		fs::path path;

	public:
		MaskLimitsIO(fs::path path) :
			path(path)
		{ }

		void save(const MaskInfo::MaskLimits& maskLimits) const
		{
			std::fstream file;
			file.open(path, std::ofstream::out);

			file << maskLimits.shapeLimits.size() << std::endl;

			for (auto& shape: maskLimits.shapeLimits)
			{
				file << shape.angleThresholds.size() << std::endl;

				for (auto threshold : shape.angleThresholds)
					Readers::ThresgoldReader::write(file, threshold);

				for (auto threshold : shape.lengthThresholds)
					Readers::ThresgoldReader::write(file, threshold);
			}

			file.close();
		}

		MaskInfo::MaskLimits load() const
		{
			MaskInfo::MaskLimits maskLimits;

			std::fstream file;
			file.open(path, std::ofstream::in);

			int shapesNumber;
			file >> shapesNumber;

			for (int i = 0; i < shapesNumber; i++)
			{
				MaskInfo::MaskShapeLimits maskShapeLimits;

				int pointsNumber;
				file >> pointsNumber;

				for (int j = 0; j < pointsNumber; j++)
					maskShapeLimits.angleThresholds.push_back(Readers::ThresgoldReader::read(file));

				for (int j = 0; j < pointsNumber; j++)
					maskShapeLimits.lengthThresholds.push_back(Readers::ThresgoldReader::read(file));

				maskLimits.shapeLimits.push_back(maskShapeLimits);
			}

			file.close();

			return maskLimits;
		}
	};
}