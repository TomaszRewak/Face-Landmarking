#pragma once 

#include <vector>
#include <filesystem>

#include "../../FaceLandmarking.Reader/tree-io.hpp"
#include "../../FaceLandmarking/math/vector.hpp"

namespace FaceLandmarking::Regression::Regressors
{
	namespace fs = std::experimental::filesystem;

	class TreeRegressor
	{
	private:
		std::vector<int> _features;
		std::vector<float> _thresholds;
		std::vector<int> _leftChildren;
		std::vector<int> _rightChildren;
		std::vector<float> _values;

	public:
		TreeRegressor(fs::path path)
		{
			Reader::TreeIO io(path);
			io.load(_features, _thresholds, _leftChildren, _rightChildren, _values);
		}

		float get(const std::vector<float>& features)
		{
			int node = 0;

			while (_features[node] != -2)
			{
				if (features[_features[node]] <= _thresholds[node])
					node = _leftChildren[node];
				else
					node = _rightChildren[node];
			}

			return _values[node];
		}
	};

	class TreeRegressorPair {
	public:
		TreeRegressor x;
		TreeRegressor y;

		TreeRegressorPair(fs::path path) : 
			x(path / "x"),
			y(path / "y")
		{ }
	};

	template<size_t N>
	class MaskTreeRegressor
	{
	private:
		std::vector<TreeRegressorPair> regressors;

	public:
		MaskTreeRegressor(fs::path basePath)
		{
			for (size_t i = 0; i < N; i++)
				regressors.push_back(TreeRegressorPair(basePath / std::to_string(i)));
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