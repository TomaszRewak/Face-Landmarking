#pragma once

#include <filesystem>
#include <fstream>

namespace FaceLandmarking::Reader
{
	namespace fs = std::experimental::filesystem;

	class FeaturesIO
	{
	private:
		std::fstream file;

	public:
		FeaturesIO() 
		{ }

		void open(size_t maskPoint, fs::path basePath)
		{
			fs::path filePath = basePath / std::to_string(maskPoint);

			file.open(filePath.string(), std::ofstream::out);
		}

		void close()
		{
			file.close();
		}

		void add(const std::vector<float>& features, const std::vector<float>& decisions)
		{
			for (auto feature : features)
				file << feature << " ";

			for (auto decision : decisions)
				file << decision << " ";

			file << "\n";
		}
	};
}