#pragma once

#include <filesystem>

#include "learning-example.hpp"
#include "../io/image-io.hpp"
#include "../io/annotation-io.hpp"

namespace FaceLandmarking::Data
{
	namespace fs = std::experimental::filesystem;

	template<size_t Nodes>
	class DatasetIterator
	{
	private:
		fs::path annotationsRootPath;
		fs::path imagesRootPath;

		std::vector<fs::path>::iterator iterator;

	public:
		DatasetIterator(			
			fs::path annotationsRootPath,
			fs::path imagesRootPath,
			std::vector<fs::path>::iterator iterator) :
			annotationsRootPath(annotationsRootPath),
			imagesRootPath(imagesRootPath),
			iterator(iterator)
		{ }

		LearningExample<Nodes> operator*()
		{
			auto maskFile = IO::AnnotationIO<Nodes>::load(annotationsRootPath / *iterator);
			auto image = IO::ImageIO::load(imagesRootPath / maskFile.imageName);

			return LearningExample<Nodes>(image, maskFile.mask);
		}

		DatasetIterator& operator++()
		{
			iterator++;

			return *this;
		}

		bool operator!=(DatasetIterator& second)
		{
			return iterator != second.iterator;
		}
	};

	template<std::size_t Nodes>
	class Dataset
	{
	private:
		fs::path annotationsRootPath;
		fs::path imagesRootPath;

		std::vector<fs::path> annotationFilePaths;

	public:
		static const size_t Nodes = 194;

		Dataset(fs::path path)
		{
			annotationsRootPath = path / "annotation";
			imagesRootPath = path / "images";

			loadAnnotationFiles();
		}

		DatasetIterator<Nodes> begin()
		{
			return DatasetIterator<Nodes>(annotationsRootPath, imagesRootPath, annotationFilePaths.begin());
		}

		DatasetIterator<Nodes> end()
		{
			return DatasetIterator<Nodes>(annotationsRootPath, imagesRootPath, annotationFilePaths.end());
		}

	private:
		void loadAnnotationFiles()
		{
			annotationFilePaths.clear();

			for (auto& file : fs::directory_iterator(annotationsRootPath))
				annotationFilePaths.push_back(file.path().filename());
		}
	};
}