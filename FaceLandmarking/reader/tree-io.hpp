#pragma once

#include <vector>
#include <filesystem>
#include <fstream>

namespace FaceLandmarking::Reader
{
	namespace fs = std::experimental::filesystem;

	class TreeIO
	{
	private:
		fs::path path;

	public:
		TreeIO(fs::path path) :
			path(path)
		{ }

		void load(
			std::vector<int>& features,
			std::vector<float>& thresholds,
			std::vector<int>& leftChildren,
			std::vector<int>& rightChildren,
			std::vector<float>& values
		) const
		{
			std::fstream file;
			file.open(path, std::ofstream::in);

			int nodes;
			file >> nodes;

			for (int j = 0; j < nodes; j++)
			{
				int feature;
				file >> feature;
				features.push_back(feature);
			}

			for (int j = 0; j < nodes; j++)
			{
				float threshold;
				file >> threshold;
				thresholds.push_back(threshold);
			}

			for (int j = 0; j < nodes; j++)
			{
				int leftChild;
				file >> leftChild;
				leftChildren.push_back(leftChild);
			}

			for (int j = 0; j < nodes; j++)
			{
				int rightChild;
				file >> rightChild;
				rightChildren.push_back(rightChild);
			}

			for (int j = 0; j < nodes; j++)
			{
				float value;
				file >> value;
				values.push_back(value);
			}

			file.close();
		}
	};
}