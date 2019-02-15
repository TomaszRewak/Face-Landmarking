#pragma once 

#include "tree-regressor.hpp"

namespace FaceLandmarking::Regression::Regressors
{
	template<size_t N>
	class TreeMaskRegressorPair {
	public:
		TreeRegressor<N> x;
		TreeRegressor<N> y;

		TreeMaskRegressorPair(fs::path path) :
			x(path / "x"),
			y(path / "y")
		{ }
	};

	template<size_t N, size_t F>
	class TreeMaskRegressor
	{
	private:
		std::vector<TreeMaskRegressorPair<F>> regressors;

	public:
		TreeMaskRegressor(fs::path basePath)
		{
			for (size_t i = 0; i < N; i++)
				regressors.push_back(TreeMaskRegressorPair<F>(basePath / std::to_string(i)));
		}

		Math::Vector<float> getOffset(int point, const std::array<float, F>& features)
		{
			return Math::Vector<float>(
				regressors[point].x.get(features),
				regressors[point].y.get(features)
				);
		}
	};
}