#pragma once

#include <istream>
#include <ostream>

#include "../../FaceLandmarking/mask-info/mask-limits.hpp"

namespace FaceLandmarking::Reader::Readers
{
	struct ThresgoldReader
	{
		static MaskInfo::Threshold read(std::istream& stream)
		{
			MaskInfo::Threshold threshold;

			size_t size;
			stream >> size;

			for (size_t i = 0; i < size; i++)
			{
				float value;
				stream >> value;

				threshold.push_back(value);
			}

			return threshold;
		}

		static void write(std::ostream& stream, const MaskInfo::Threshold& threshold)
		{
			stream << threshold.size() << " ";

			for (float value : threshold)
				stream << value << " ";

			stream << "\n";
		}
	};
}