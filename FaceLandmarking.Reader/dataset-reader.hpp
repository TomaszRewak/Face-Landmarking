#pragma once

#include <filesystem>

#include "learning-example.hpp"
#include "readers/image-reader.hpp"
#include "readers/mask-reader.hpp"

namespace FaceLandmarking::Reader
{
	namespace fs = std::experimental::filesystem;

	template<size_t N>
	class DatasetReader
	{
	private:
		fs::path annotationsRootPath;
		fs::path imagesRootPath;

		std::vector<fs::path> annotationFilePaths;
		std::vector<fs::path>::iterator exampleIterator;

	public:
		DatasetReader(fs::path path)
		{
			annotationsRootPath = path / "annotation";
			imagesRootPath = path / "images";

			loadAnnotationFiles();
		}

		bool hasNext() const
		{
			return exampleIterator < annotationFilePaths.end();
		}

		LearningExample<N> loadNext(bool includeImage = true)
		{
			auto[mask, imageName] = MaskReader<N>::loadMask(*exampleIterator);

			cv::Mat image;

			if (includeImage)
			{
				fs::path imagePath = imagesRootPath / (imageName + ".jpg");
				image = ImageReader::loadImage(imagePath);
			}

			exampleIterator++;

			return LearningExample<N>(image, mask);
		}

	private:
		void loadAnnotationFiles()
		{
			annotationFilePaths.clear();

			for (auto& file : fs::directory_iterator(annotationsRootPath))
				annotationFilePaths.push_back(file.path());

			exampleIterator = annotationFilePaths.begin();
		}
	};
}