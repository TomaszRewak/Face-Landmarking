#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../../FaceLandmarking/math/rect.hpp"

namespace FaceLandmarking::App::UI
{
	class FaceUI
	{
	public:
		static void drawFace(cv::Mat& image, Math::Rect<float> rect, cv::Scalar color = cv::Scalar(0, 0, 255))
		{
			cv::rectangle(image, 
				cv::Point(rect.center.x - rect.size.width / 2, rect.center.y - rect.size.height / 2),
				cv::Point(rect.center.x + rect.size.width / 2, rect.center.y + rect.size.height / 2), 
				color, 2);
		}
	};
}