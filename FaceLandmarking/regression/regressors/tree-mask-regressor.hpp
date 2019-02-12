#pragma once 

#include "tree-regressor.hpp"

namespace FaceLandmarking::Regression::Regressors
{
	class TreeMaskRegressorPair {
	public:
		TreeRegressor x;
		TreeRegressor y;

		TreeMaskRegressorPair(fs::path path) :
			x(path / "x"),
			y(path / "y")
		{ }
	};

	template<size_t N>
	class TreeMaskRegressor
	{
	private:
		std::vector<TreeMaskRegressorPair> regressors;

	public:
		TreeMaskRegressor(fs::path basePath)
		{
			for (size_t i = 0; i < N; i++)
				regressors.push_back(TreeMaskRegressorPair(basePath / std::to_string(i)));
		}

		Math::Vector<float> getOffset(int point, const std::vector<float>& features)
		{
			return Math::Vector<float>(
				regressors[point].x.get(features),
				regressors[point].y.get(features)
				);
		}
	};
}