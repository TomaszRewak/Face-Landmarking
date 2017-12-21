#pragma once

#include <vector>
#include "../FaceLandmarking.Reader/nn-oi.hpp"

namespace FaceLandmarking::Learning
{
	namespace fs = std::experimental::filesystem;

	class MaskPointRegression
	{
	private:
		std::vector<std::vector<float>> weights;
		std::vector<std::vector<float>> biases;
		std::vector<std::vector<float>> layers;

	public:
		MaskPointRegression(fs::path path)
		{
			Reader::NNIO io(path);

			auto layerSizes = io.load(weights, biases);

			for (auto size : layerSizes)
				layers.push_back(std::vector<float>(size));
		}

		void get(const std::vector<float>& features, std::vector<float>& output)
		{
			for (size_t i = 0; i < features.size(); i++)
				layers[0][i] = features[i];

			for (size_t layer = 0; layer < weights.size(); layer++)
			{
				auto& l0 = layers[layer];
				auto& l1 = layers[layer + 1];
				auto& w = weights[layer];
				auto& b = biases[layer];

				size_t l0_s = l0.size();
				size_t l1_s = l1.size();
				size_t w_s = w.size();
				size_t b_s = b.size();

				for (auto& n : l1)
					n = 0;

				for (size_t w_i = 0; w_i < w_s; w_i++)
					l1[w_i % l1_s] += w[w_i] * l0[w_i / l0_s];

				for (size_t b_i = 0; b_i < b_s; b_i++)
					l1[b_i] += b[b_i];

				for (auto& n : l1)
					n = 1 / (1 + std::exp(-n));
			}

			for (auto v : layers[layers.size() - 1])
				output.push_back(v);
		}
	};

	class MaskRegression
	{
	private:
		std::vector<MaskPointRegression> regressors;

	public:
		MaskRegression(fs::path basePath, size_t pointsNumber = 194)
		{
			for (size_t i = 0; i < pointsNumber; i++)
			{
				regressors.push_back(MaskPointRegression(basePath / std::to_string(i)));
			}
		}

		void get(size_t point, const std::vector<float>& features, std::vector<float>& output)
		{
			regressors[point].get(features, output);
		}
	};
}