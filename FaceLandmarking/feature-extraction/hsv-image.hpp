#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <array>

namespace FaceLandmarking::FeatureExtraction
{
	enum class HsvChannel
	{
		H = 0,
		S = 1,
		V = 2
	};

	const std::array<HsvChannel, 3> HsvChannels
	{ HsvChannel::H, HsvChannel::S, HsvChannel::V };

	class HsvImage
	{
	public:
		cv::Mat color;
		cv::Mat hsv[3];

	public:
		void setImage(const cv::Mat& image)
		{
			cv::cvtColor(image, color, cv::COLOR_BGR2HSV_FULL);
			cv::split(color, hsv);
		}

		void getImage(cv::Mat& image) const
		{
			cv::merge(hsv, 3, color);
			cv::cvtColor(color, image, cv::COLOR_HSV2BGR_FULL);
		}

		cv::Mat& operator[](HsvChannel channel)
		{
			return hsv[(int)channel];
		}

		const cv::Mat& operator[](HsvChannel channel) const
		{
			return hsv[(int)channel];
		}

		void add(HsvChannel channel, int offset)
		{
			cv::Mat& image = (*this)[channel];

			for (size_t x = 0; x < image.cols; x++) {
				for (size_t y = 0; y < image.rows; y++)
				{
					auto& pixel = image.at<uchar>(y, x);
					pixel = std::max(std::min(pixel + offset, 255), 0);
				}
			}
		}

		void addOffset(HsvChannel channel, int offset)
		{
			cv::Mat& image = (*this)[channel];

			while (offset < 0)
				offset += 256;
			while (offset >= 256)
				offset -= 256;

			for (size_t x = 0; x < image.cols; x++) {
				for (size_t y = 0; y < image.rows; y++)
				{
					auto& pixel = image.at<uchar>(y, x);
					pixel = (pixel + offset) % 256;
				}
			}
		}

		size_t columns() const
		{
			return color.cols;
		}

		size_t rows() const
		{
			return color.rows;
		}
	};
}