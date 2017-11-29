#pragma once

#include <fstream>
#include <string>

#include "./mask-file.hpp"

namespace FaceLandmarking::Reader {
	class MaskReader {
	private:
		static void parseMask(std::fstream& file, FaceMask& mask)
		{
			float x, y;
			std::string _;

			while (file >> x >> _ >> y)
				mask.push_back(Math::Point<float>(x, y));
		}

	public:
		static MaskFile loadMask(fs::path filePath) 
		{
			std::fstream file;
			MaskFile maskFile;

			file.open(filePath.string());

			file >> maskFile.image;
			parseMask(file, maskFile.mask);

			file.close();

			return maskFile;
		}
	};
}