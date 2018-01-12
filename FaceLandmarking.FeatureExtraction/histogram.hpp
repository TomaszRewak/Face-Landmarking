#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>

#include "../FaceLandmarking/math/rect.hpp"

namespace FaceLandmarking::FeatureExtraction
{
	class Histogram : public std::vector<float>
	{
	public:
		Histogram(int size = 256) :
			std::vector<float>(size, 0)
		{ }

		void setImage(cv::Mat& image, Math::Rect<float> rect, cv::Mat& mask)
		{
			std::fill(begin(), end(), 0);

			int
				x1 = std::max((int)rect.left(), 0),
				x2 = std::min((int)rect.right(), image.cols),
				y1 = std::max((int)rect.top(), 0),
				y2 = std::min((int)rect.down(), image.rows);

			for (int x = x1; x < x2; x++)
			{
				for (int y = y1; y < y2; y++)
				{
					cv::Vec3b a;
					cv::Vec3b b;

					cv::Vec3b c = a + b;

					auto value = image.at<uchar>(y, x);

					if (size() < value + 1)
						resize(value + 1, 0);

					(*this)[value] += mask.at<float>(y, x);
				}
			}
		}

		int max(int neighborhood = 0, bool includeEnds = true) const
		{
			int first = neighborhood;
			int last = size() - 1 - neighborhood;

			if (!includeEnds)
			{
				first += 1 + neighborhood;
				last -= 1 + neighborhood;
			}

			int max = 0;
			double maxValue = -1;

			for (int i = first; i <= last; i++)
			{
				double neighborhoodSum = 0;

				for (int j = -neighborhood; j <= neighborhood; j++)
					neighborhoodSum += (*this)[i + j] * (neighborhood - std::abs(j));

				if (neighborhoodSum > maxValue)
				{
					max = i;
					maxValue = neighborhoodSum;
				}
			}

			return max;
		}

		int median() const
		{
			float sum = 0;
			for (auto& value : *this)
				sum += value;

			float acc = 0;

			for (int i = 0; i < size(); i++)
			{
				acc += (*this)[i];

				if (acc >= sum / 2)
					return i;
			}

			return -1;
		}
	};
}