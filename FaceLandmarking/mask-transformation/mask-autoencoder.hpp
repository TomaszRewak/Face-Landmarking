#pragma once

#include "../face-mask.hpp"
#include "mask-normalizer.hpp"

namespace FaceLandmarking::MaskTransformation
{
	template<size_t N, typename Regressor>
	class MaskAutoencoder
	{
	private:
		Regressor regressor;

	public:
		MaskAutoencoder(Regressor regressor) :
			regressor(regressor)
		{ }

		FaceMask<N> passThrough(const FaceMask<N>& input)
		{
			auto inputRect = input.faceRect();
			auto normalizedInput = MaskNormalizer<N>::normalizeMask(input);
			auto normalizedInputRect = normalizedInput.faceRect();

			std::array<float, N * 2> inputValues;
			std::array<float, N * 2> outputValues;

			for (int i = 0; i < N; i++)
			{
				inputValues[i * 2] = normalizedInput[i].x;
				inputValues[i * 2 + 1] = normalizedInput[i].y;
			}

			regressor.passThrough(inputValues.begin(), outputValues.begin());

			auto normalizedOutput = normalizedInput;
			for (int i = 0; i < N; i++)
			{
				normalizedOutput[i].x = outputValues[i * 2];
				normalizedOutput[i].y = outputValues[i * 2 + 1];
			}

			auto output = MaskNormalizer<N>::normalizeMask(normalizedOutput, normalizedInputRect, inputRect);

			return output;
		}
	};
}