#pragma once

#include <filesystem>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "../math/rect.hpp"

namespace FaceLandmarking::Preprocessing
{
	namespace fs = std::experimental::filesystem;

	class FaceFinder 
	{
	private:
		cv::CascadeClassifier faceCascade;
		std::vector<cv::Rect> faceRects;
		cv::Mat grayImage;

	public:
		FaceFinder(fs::path faceCascadePath) :
			faceCascade(faceCascadePath.string())
		{ }

		std::vector<Math::Rect<float>> locate(cv::Mat& image)
		{
			std::vector<Math::Rect<float>> result;

			faceRects.clear();

			cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
			faceCascade.detectMultiScale(grayImage, faceRects, 1.3, 5);

			for (auto& face : faceRects) {
				Math::Rect<float> rect(face.x, face.y, face.width, face.height);
				rect.center += Math::Vector<float>(0, rect.size.height / 8);

				result.push_back(rect);
			}

			return result;
		}
	};
}