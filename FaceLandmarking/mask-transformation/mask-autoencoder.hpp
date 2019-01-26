#pragma once

#include "../face-mask.hpp"
#include "mask-normalizer.hpp"

namespace FaceLandmarking::MaskTransformation
{
	template<typename Regressor>
	class MaskAutoencoder
	{
	private:
		Regressor regressor;

	public:
		MaskAutoencoder(Regressor regressor) :
			regressor(regressor)
		{ }

		FaceMask passThrough(const FaceMask& input)
		{
			auto inputRect = input.faceRect();
			auto normalizedInput = MaskNormalizer::normalizeMask(input);
			auto normalizedInputRect = normalizedInput.faceRect();

			std::vector<float> inputValues(input.size() * 2);
			std::vector<float> outputValues(input.size() * 2);

			for (int i = 0; i < normalizedInput.size(); i++)
			{
				inputValues[i * 2] = normalizedInput[i].x;
				inputValues[i * 2 + 1] = normalizedInput[i].y;
			}

			regressor.passThrough(inputValues.begin(), outputValues.begin());

			auto normalizedOutput = normalizedInput;
			for (int i = 0; i < normalizedOutput.size(); i++)
			{
				normalizedOutput[i].x = outputValues[i * 2];
				normalizedOutput[i].y = outputValues[i * 2 + 1];
			}

			auto output = MaskNormalizer::normalizeMask(normalizedOutput, normalizedInputRect, inputRect);

			return output;
		}
	};
}