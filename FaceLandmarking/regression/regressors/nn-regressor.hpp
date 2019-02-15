#pragma once 

#include <vector>
#include <filesystem>
#include <cassert>

#include "../../io/nn-oi.hpp"

namespace FaceLandmarking::Regression::Regressors
{
	namespace fs = std::experimental::filesystem;

	struct IdentityActivation
	{
		float operator()(float value) { return value; }
	};

	struct ReluActivation
	{
		float operator()(float value) { return std::max(0.f, value); }
	};

	struct LogisticActivation
	{
		float operator()(float value) { return 1.f / (1.f + std::exp(-value)); }
	};

	struct TanhActivation
	{
		float operator()(float value) { return std::tanh(value); }
	};

	template<typename Activation>
	class NNRegressor
	{
	private:
		std::vector<std::vector<float>> weights;
		std::vector<std::vector<float>> biases;
		std::vector<std::size_t> layers;

	public:
		NNRegressor(fs::path path)
		{
			IO::NNIO::load(path, layers, weights, biases);
		}

		template<typename Iterator>
		void operator()(Iterator input, Iterator output)
		{
			Activation activation;

			for (size_t i = 0; i < layers[0].size(); i++)
				layers[0][i] = *(input++);

			for (size_t layer = 0; layer < layers.size() - 1; layer++)
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
					l1[w_i % l1_s] += w[w_i] * l0[w_i / l1_s];

				for (size_t b_i = 0; b_i < b_s; b_i++)
					l1[b_i] += b[b_i];

				if (layer < layers.size() - 2)
					for (auto& n : l1)
						n = activation(n);
			}

			for (auto v : layers[layers.size() - 1])
				*(output++) = v;
		}
	};
}