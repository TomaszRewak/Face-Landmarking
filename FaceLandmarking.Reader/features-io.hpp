#pragma once

#include <filesystem>
#include <fstream>

namespace FaceLandmarking::Reader
{
	namespace fs = std::experimental::filesystem;

	class FeaturesIO
	{
	private:
		fs::path basePath;

	public:
		FeaturesIO(fs::path basePath) :
			basePath(basePath)
		{ }

		void save(size_t maskPoint, const std::vector<float>& features, const std::vector<float>& decisions) const
		{
			fs::create_directory(basePath);
			fs::path filePath = basePath / std::to_string(maskPoint);

			std::fstream file;
			file.open(filePath.string(), std::ofstream::out | std::ofstream::app);

			for (auto feature : features)
				file << feature << " ";

			for (auto decision : decisions)
				file << decision << " ";

			file << "\n";
			file.close();
		}
	};
}