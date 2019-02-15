#pragma once

#include "dataset.hpp"

namespace FaceLandmarking::Data
{
	namespace fs = std::experimental::filesystem;

	template<std::size_t Nodes, typename DatasetIterator>
	class DatasetMirroringIterator
	{
	private:
		DatasetIterator iterator;
		bool reversed;

	public:
		DatasetMirroringIterator(DatasetIterator iterator) :
			iterator(iterator),
			reversed(false)
		{ }

		LearningExample<Nodes> operator*()
		{
			auto example = *iterator;

			if (!reversed)
				return example;

			auto flippedImage = flipImage(example.image);
			auto flippedMask = flipMask(example.mask, example.image.cols);

			return LearningExample<Nodes>(
				flippedImage,
				flippedMask);
		}

		DatasetMirroringIterator& operator++()
		{
			if (reversed)
			{
				++iterator;
				reversed = false;
			}
			else
			{
				reversed = true;
			}

			return *this;
		}

		bool operator!=(DatasetMirroringIterator& second)
		{
			return iterator != second.iterator || reversed != reversed;
		}

	private:
		cv::Mat flipImage(const cv::Mat& image)
		{
			cv::Mat flippedImage;
			cv::flip(image, flippedImage, 1);

			return flippedImage;
		}

		Mask::FaceMask<Nodes> flipMask(Mask::FaceMask<Nodes>& mask, std::size_t width)
		{
			Mask::FaceMask<Nodes> flippedMask = mask;
			for (auto& point : flippedMask)
				point.x = width - point.x;

			return flippedMask;
		}
	};
}