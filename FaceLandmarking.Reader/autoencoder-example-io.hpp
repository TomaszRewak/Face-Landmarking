#pragma once

#include <fstream>

#include "../FaceLandmarking/face-mask.hpp"

namespace FaceLandmarking::Reader
{
	namespace fs = std::experimental::filesystem;
	
	template<size_t N>
	class AutoencoderExampleIO
	{
	private:
		std::fstream file;

	public:
		AutoencoderExampleIO()
		{ }

		void open(fs::path filePath)
		{
			file.open(filePath.string(), std::ofstream::out);
		}

		void close()
		{
			file.close();
		}

		void add(const FaceMask<N>& inputMask, const FaceMask<N>& outputMask)
		{
			for (auto point : inputMask)
				file << point.x << " " << point.y << " ";

			file << "\n";

			for (auto point : outputMask)
				file << point.x << " " << point.y << " ";

			file << "\n";
		}
	};
}