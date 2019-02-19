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

		// --> jaw 0 40
		// --> nose 41 57
		// --> outer upper lip 58 71 (include edge)
		// <-- outer lower lip 72 85
		// --> inner upper lip 86 100 (include edge)
		// <-- inner lower lip 101 113
		// --> left upper eye 114 124
		// <-- left lower eye 125 133
		// <-- right upper eye 134 144
		// --> right lower eye 145 153
		// --> left upper eyebrow 154 164
		// <-- left lower eyebrow 165 173
		// <-- right upper eyebrow 174 184
		// --> right lower eyebrow 185 193

		Mask::FaceMask<Nodes> flipMask(Mask::FaceMask<Nodes>& mask, std::size_t width)
		{
			Mask::FaceMask<Nodes> flippedMask = mask;

			for (auto& point : flippedMask)
				point.x = width - point.x;

			for (int i = 0; i <= 40; i++)
				std::swap(flippedMask[i], flippedMask[40 - i]);
			for (int i = 41; i <= 57; i++)
				std::swap(flippedMask[i], flippedMask[57 - (i - 41)]);
			for (int i = 58; i <= 71; i++)
				std::swap(flippedMask[i], flippedMask[71 - (i - 58)]);
			for (int i = 72; i <= 85; i++)
				std::swap(flippedMask[i], flippedMask[85 - (i - 72)]);
			for (int i = 86; i <= 100; i++)
				std::swap(flippedMask[i], flippedMask[100 - (i - 86)]);
			for (int i = 101; i <= 113; i++)
				std::swap(flippedMask[i], flippedMask[113 - (i - 101)]);
			for (int i = 114; i <= 124; i++)
				std::swap(flippedMask[i], flippedMask[134 + (i - 114)]);
			for (int i = 125; i <= 133; i++)
				std::swap(flippedMask[i], flippedMask[145 + (i - 125)]);
			for (int i = 154; i <= 164; i++)
				std::swap(flippedMask[i], flippedMask[174 + (i - 154)]);
			for (int i = 165; i <= 173; i++)
				std::swap(flippedMask[i], flippedMask[185 + (i - 165)]);

			return flippedMask;
		}
	};
}