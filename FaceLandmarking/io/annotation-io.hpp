#pragma once

#include <fstream>
#include <string>

#include "../data/mask-file.hpp"

namespace FaceLandmarking::IO
{
	namespace fs = std::experimental::filesystem;

	template<size_t N>
	class AnnotationIO
	{
	private:
		static void parseMask(std::fstream& file, Mask::FaceMask<N>& mask)
		{
			float x, y;
			std::string _;

			for (size_t i = 0; i < N; i++) {
				file >> x >> _ >> y;
				mask[i] = Math::Point<float>(x, y);
			}
		}

	public:
		static Data::MaskFile<N> load(fs::path filePath)
		{
			std::fstream file;
			Mask::MaskFile<N> maskFile;

			file.open(filePath.string());

			file >> maskFile.imageName;
			parseMask(file, maskFile.mask);

			file.close();

			return maskFile;
		}
	};
}