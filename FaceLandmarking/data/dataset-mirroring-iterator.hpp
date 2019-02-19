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
			{
				return example;
			}
			else
			{
				auto flippedImage = flipImage(example.image);
				auto flippedMask = flipMask(example.mask, example.image.cols);

				return LearningExample<Nodes>(
					flippedImage,
					flippedMask);

			}
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
			Mask::FaceMask<Nodes> flippedMask;

			for (int i = 0; i <= 40; i++)
				flippedMask[i] = mask[40 - i];
			for (int i = 41; i <= 57; i++)
				flippedMask[i] = mask[57 - (i - 41)];
			for (int i = 58; i <= 71; i++)
				flippedMask[i] = mask[71 - (i - 58)];
			for (int i = 72; i <= 85; i++)
				flippedMask[i] = mask[85 - (i - 72)];
			for (int i = 86; i <= 100; i++)
				flippedMask[i] = mask[100 - (i - 86)];
			for (int i = 101; i <= 113; i++)
				flippedMask[i] = mask[113 - (i - 101)];
			for (int i = 114; i <= 124; i++)
				flippedMask[i] = mask[134 + (i - 114)];
			for (int i = 125; i <= 133; i++)
				flippedMask[i] = mask[145 + (i - 125)];
			for (int i = 134; i <= 144; i++)
				flippedMask[i] = mask[114 + (i - 134)];
			for (int i = 145; i <= 153; i++)
				flippedMask[i] = mask[125 + (i - 145)];
			for (int i = 154; i <= 164; i++)
				flippedMask[i] = mask[174 + (i - 154)];
			for (int i = 165; i <= 173; i++)
				flippedMask[i] = mask[185 + (i - 165)];
			for (int i = 174; i <= 184; i++)
				flippedMask[i] = mask[154 + (i - 174)];
			for (int i = 185; i <= 193; i++)
				flippedMask[i] = mask[165 + (i - 185)];

			for (auto& point : flippedMask)
				point.x = width - point.x;

			return flippedMask;
		}
	};
}