#pragma once

#include "../mask/mask-transformation/mask-normalizer.hpp"

namespace FaceLandmarking::Regression
{
	template<std::size_t Nodes, typename Regressor>
	class MaskAutoencoder
	{
	public:
		MaskAutoencoder(Regressor regressor) :
			_regressor(regressor)
		{ }

		Mask::FaceMask<Nodes> operator()(const Mask::FaceMask<Nodes>& input)
		{
			auto inputRect = input.faceRect();
			auto normalizedInput = Mask::MaskTransformation::MaskNormalizer<Nodes>()(input);
			auto normalizedInputRect = normalizedInput.faceRect();

			std::array<float, Nodes * 2> inputValues;
			std::array<float, Nodes * 2> outputValues;

			for (int i = 0; i < Nodes; i++)
			{
				inputValues[i * 2] = normalizedInput[i].x;
				inputValues[i * 2 + 1] = normalizedInput[i].y;
			}

			regressor(inputValues.begin(), outputValues.begin());

			auto normalizedOutput = normalizedInput;
			for (int i = 0; i < Nodes; i++)
			{
				normalizedOutput[i].x = outputValues[i * 2];
				normalizedOutput[i].y = outputValues[i * 2 + 1];
			}

			auto output = MaskNormalizer<Nodes>()(normalizedOutput, normalizedInputRect, inputRect);

			return output;
		}

	private:
		Regressor regressor;
	};
}