#pragma once

#include <fstream>
#include <string>

#include "./mask-file.hpp"

namespace FaceLandmarking::Reader
{
	template<size_t N>
	class MaskReader
	{
	private:
		static void parseMask(std::fstream& file, FaceMask<N>& mask)
		{
			float x, y;
			std::string _;

			for (size_t i = 0; i < N; i++) {
				file >> x >> _ >> y;
				mask[i] = Math::Point<float>(x, y);
			}
		}

	public:
		static MaskFile<N> loadMask(fs::path filePath)
		{
			std::fstream file;
			MaskFile<N> maskFile;

			file.open(filePath.string());

			file >> maskFile.imageName;
			parseMask(file, maskFile.mask);

			file.close();

			return maskFile;
		}
	};
}