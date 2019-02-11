#pragma once

#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

namespace FaceLandmarking::IO
{
	namespace fs = std::experimental::filesystem;

	class ImageIO
	{
	public:
		static cv::Mat load(fs::path filePath)
		{
			cv::Mat image;

			image = cv::imread(filePath.string(), cv::IMREAD_COLOR);

			return image;
		}
	};
}