#pragma once

#include <vector>
#include <filesystem>
#include <fstream>

namespace FaceLandmarking::Reader
{
	namespace fs = std::experimental::filesystem;

	class NNIO
	{
	private:
		fs::path path;

	public:
		NNIO(fs::path path) :
			path(path)
		{ }

		std::vector<size_t> load(std::vector<std::vector<float>>& weights, std::vector<std::vector<float>>& biases) const
		{
			std::fstream file;
			file.open(path, std::ofstream::in);

			int layers;
			file >> layers;

			std::vector<size_t> layerSizes;
			for (int i = 0; i < layers; i++)
			{
				size_t layerSize;
				file >> layerSize;
				layerSizes.push_back(layerSize);
			}

			for (int i = 0; i < layers - 1; i++)
			{
				size_t l1 = layerSizes[i];
				size_t l2 = layerSizes[i + 1];

				std::vector<float> ws;
				std::vector<float> bs;

				for (size_t j = 0; j < l1 * l2; j++)
				{
					float w;
					file >> w;
					ws.push_back(w);
				}

				for (size_t j = 0; j < l2; j++)
				{
					float b;
					file >> b;
					bs.push_back(b);
				}

				weights.push_back(ws);
				biases.push_back(bs);
			}

			file.close();

			return layerSizes;
		}
	};
}