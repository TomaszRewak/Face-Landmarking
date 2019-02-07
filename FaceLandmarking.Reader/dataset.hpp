#pragma once

#include <filesystem>

#include "learning-example.hpp"
#include "readers/image-reader.hpp"
#include "readers/mask-reader.hpp"

namespace FaceLandmarking::Reader
{
	namespace fs = std::experimental::filesystem;

	template<size_t Nodes>
	class DatasetIterator
	{
	private:
		std::vector<fs::path>::iterator iterator;

	public:
		LearningExample<Nodes> operator*()
		{
			MaskFile<N> maskFile = MaskReader<Nodes>::loadMask(*iterator);
			cv::Mat image = ImageReader::loadImage(getImagePath(*iterator, maskFile.imageName));

			iterator++;

			return LearningExample<Nodes>(image, maskFile.mask);
		}

	private:
		fs::path getImagePath(std::string imageName)
		{
			return iterator->parent_path() / ".." / "images" / (imageName + ".jpg")
		}
	};

	class Dataset
	{
	private:
		fs::path annotationsRootPath;
		fs::path imagesRootPath;

		std::vector<fs::path> annotationFilePaths;
		std::vector<fs::path>::iterator exampleIterator;

	public:
		static const size_t Nodes = 194;

		Dataset(fs::path path)
		{
			path.

			annotationsRootPath = path / "annotation";
			imagesRootPath = path / "images";

			loadAnnotationFiles();
		}

		bool hasNext() const
		{
			return exampleIterator < annotationFilePaths.end();
		}

		void 

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