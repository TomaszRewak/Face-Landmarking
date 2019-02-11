#pragma once

#include <filesystem>
#include <fstream>

namespace FaceLandmarking::IO
{
	namespace fs = std::experimental::filesystem;

	class FeaturesIO
	{
	private:
		std::fstream file;

	public:
		FeaturesIO() 
		{ }

		void open(fs::path basePath, size_t maskPoint)
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