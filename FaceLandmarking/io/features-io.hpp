#pragma once

#include <filesystem>
#include <fstream>

#include "../math/vector.hpp"

namespace FaceLandmarking::IO
{
	namespace fs = std::experimental::filesystem;
	
	template<std::size_t Length>
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

		void add(const std::array<float, Length>& features, const Math::Vector<float>& decision)
		{
			for (auto feature : features)
				file << feature << " ";

			file << decision.x << " " << decision.y;

			file << "\n";
		}
	};
}