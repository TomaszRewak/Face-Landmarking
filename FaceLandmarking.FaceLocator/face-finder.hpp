#pragma once

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "../FaceLandmarking/math/rect.hpp"

namespace FaceLandmarking::FaceLocator
{
	class FaceFinder : public std::vector<Math::Rect<float>>
	{
	private:
		cv::CascadeClassifier faceCascade;
		std::vector<cv::Rect> faceRects;
		cv::Mat grayImage;

	public:
		FaceFinder(std::string faceCascadePath) :
			faceCascade(faceCascadePath)
		{ }

		void locate(cv::Mat& image)
		{
			clear();
			faceRects.clear();

			cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
			faceCascade.detectMultiScale(grayImage, faceRects, 1.3, 5);

			for (auto& face : faceRects) {
				Math::Rect<float> rect(face.x, face.y, face.width, face.height);
				rect.center += Math::Vector<float>(0, rect.size.height / 8);

				push_back(rect);
			}
		}
	};
}