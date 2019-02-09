#pragma once

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

namespace FaceLandmarking::Reader
{
	namespace fs = std::experimental::filesystem;

	class ImageReader
	{
	public:
		static cv::Mat loadImage(fs::path filePath)
		{
			cv::Mat image;

			image = cv::imread(filePath.string(), cv::IMREAD_COLOR);

			return image;
		}
	};
}