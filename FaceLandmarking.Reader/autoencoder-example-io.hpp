#pragma once

#include <fstream>

#include "../FaceLandmarking/face-mask.hpp"

namespace FaceLandmarking::Reader
{
	namespace fs = std::experimental::filesystem;
	
	class AutoencoderExampleIO
	{
	private:
		std::fstream file;

	public:
		AutoencoderExampleIO() :
		{ }

		void open(fs::path filePath)
		{
			file.open(filePath.string(), std::ofstream::out);
		}

		void close()
		{
			file.close();
		}

		void add(const FaceMask& inputMask, const FaceMask& outputMask)
		{
			for (auto feature : inputMask)
				file << feature.x << " " << feature.y << " ";

			file << "\n";

			for (auto feature : outputMask)
				file << feature.x << " " << feature.y << " ";

			file << "\n";
		}
	};
}