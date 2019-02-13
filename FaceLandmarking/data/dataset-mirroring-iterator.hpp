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
			auto flippedMask = flipMask(example.mask);

			return example LearningExample<Nodes>(
				flippedImage,
				flippedMask);
		}

		DatasetIterator& operator++()
		{
			if (reversed)
			{
				iterator++;
				reversed = false;
			}
			else
			{
				reversed = true;
			}
		}

		bool operator!=(DatasetIterator& second)
		{
			return iterator != second.iterator || reversed != reversed;
		}

	private:
		cv::Mat flipImage(const cv::Mat& image)
		{
			cv::Mat flippedImage;
			cv::flip(example.image, flippedImage, 1);

			return flippedImage;
		}

		Mask::FaceMask<Nodes> flipMask(const Mask::FaceMask<Nodes>& mask)
		{
			Mask::FaceMask<Nodes> flippedMask = mask;
			for (auto& point : flippedMask)
				point.x = src.cols - point.x;

			return flippedMask;
		}
	};
}